//
// Copyright (C) 2020 OpenSim Ltd.
//
// SPDX-License-Identifier: LGPL-3.0-or-later
//


#include "inet/queueing/queue/PacketQueue.h"

#include "inet/common/ModuleAccess.h"
#include "inet/common/PacketEventTag.h"
#include "inet/common/Simsignals.h"
#include "inet/common/TimeTag.h"
#include "inet/queueing/function/PacketComparatorFunction.h"
#include "inet/queueing/function/PacketDropperFunction.h"

/*
 * @sqsq
 */
#include "inet/routing/ospfv2/router/Ospfv2Common.h"

namespace inet {
namespace queueing {

Define_Module(PacketQueue);

/*
 * @sqsq
 */
PacketQueue::~PacketQueue()
{
    delete packetDropperFunction;
    ofs.close();
}

void PacketQueue::initialize(int stage)
{
    PacketQueueBase::initialize(stage);
    if (stage == INITSTAGE_LOCAL) {
        queue.setName("storage");
        producer = findConnectedModule<IActivePacketSource>(inputGate);
        collector = findConnectedModule<IActivePacketSink>(outputGate);
        packetCapacity = par("packetCapacity");
        dataCapacity = b(par("dataCapacity"));
        buffer = findModuleFromPar<IPacketBuffer>(par("bufferModule"), this);
        packetComparatorFunction = createComparatorFunction(par("comparatorClass"));
        if (packetComparatorFunction != nullptr)
            queue.setup(packetComparatorFunction);
        packetDropperFunction = createDropperFunction(par("dropperClass"));
    }
    else if (stage == INITSTAGE_QUEUEING) {
        checkPacketOperationSupport(inputGate);
        checkPacketOperationSupport(outputGate);
        if (producer != nullptr)
            producer->handleCanPushPacketChanged(inputGate->getPathStartGate());
    }
    else if (stage == INITSTAGE_LAST)
        updateDisplayString();

    /*
     * @sqsq
     */
    std::string filename = "/home/sqsq/Desktop/"
            "sat-ospf/inet/examples/ospfv2/sqsqtest/results/";
    filename += EXPERIMENT_NAME;
    filename += "/";
    filename += std::to_string(SQSQ_HOP);
    filename += "/";
    filename += getEnvir()->getConfigEx()->getActiveConfigName();
    filename += "/dropPacketRaw.csv";
    ofs.open(filename, std::ios::app);
}

IPacketDropperFunction *PacketQueue::createDropperFunction(const char *dropperClass) const
{
    if (strlen(dropperClass) == 0)
        return nullptr;
    else
        return check_and_cast<IPacketDropperFunction *>(createOne(dropperClass));
}

IPacketComparatorFunction *PacketQueue::createComparatorFunction(const char *comparatorClass) const
{
    if (strlen(comparatorClass) == 0)
        return nullptr;
    else
        return check_and_cast<IPacketComparatorFunction *>(createOne(comparatorClass));
}

bool PacketQueue::isOverloaded() const
{
    return (packetCapacity != -1 && getNumPackets() > packetCapacity) ||
           (dataCapacity != b(-1) && getTotalLength() > dataCapacity);
}

int PacketQueue::getNumPackets() const
{
    return queue.getLength();
}

Packet *PacketQueue::getPacket(int index) const
{
    if (index < 0 || index >= queue.getLength())
        throw cRuntimeError("index %i out of range", index);
    return check_and_cast<Packet *>(queue.get(index));
}

void PacketQueue::pushPacket(Packet *packet, cGate *gate)
{
    Enter_Method("pushPacket");

    /*
     * @sqsq
     */
    checkAndEmitQueueLoadLevel(packet);

    take(packet);
    cNamedObject packetPushStartedDetails("atomicOperationStarted");
    emit(packetPushStartedSignal, packet, &packetPushStartedDetails);
    EV_INFO << "Pushing packet" << EV_FIELD(packet) << EV_ENDL;
    queue.insert(packet);
    if (buffer != nullptr)
        buffer->addPacket(packet);
    else if (packetDropperFunction != nullptr) {
        while (isOverloaded()) {
            auto packet = packetDropperFunction->selectPacket(this);

            /*
             * @sqsq
             */
            ofs << getEnvir()->getConfigEx()->getActiveConfigName() << ",";
            ofs << SQSQ_HOP << ",";
            ofs << this->getParentModule()->getFullPath() << ",";
            ofs << simTime() << ",";
            ofs << 0 << ",";
            ofs << 0 << ",";
            ofs << 0 << ",";
            ofs << 1;
            ofs << std::endl;

            EV_INFO << "Dropping packet" << EV_FIELD(packet) << EV_ENDL;
            queue.remove(packet);
            dropPacket(packet, QUEUE_OVERFLOW);
        }
    }
    ASSERT(!isOverloaded());
    if (collector != nullptr && getNumPackets() != 0)
        collector->handleCanPullPacketChanged(outputGate->getPathEndGate());
    cNamedObject packetPushEndedDetails("atomicOperationEnded");
    emit(packetPushEndedSignal, nullptr, &packetPushEndedDetails);
    updateDisplayString();
}

Packet *PacketQueue::pullPacket(cGate *gate)
{
    Enter_Method("pullPacket");
    auto packet = check_and_cast<Packet *>(queue.front());

    /*
     * @sqsq
     */
    checkAndEmitQueueLoadLevel(packet);

    EV_INFO << "Pulling packet" << EV_FIELD(packet) << EV_ENDL;
    if (buffer != nullptr) {
        queue.remove(packet);
        buffer->removePacket(packet);
    }
    else
        queue.pop();
    auto queueingTime = simTime() - packet->getArrivalTime();
    auto packetEvent = new PacketQueuedEvent();
    packetEvent->setQueuePacketLength(getNumPackets());
    packetEvent->setQueueDataLength(getTotalLength());
    insertPacketEvent(this, packet, PEK_QUEUED, queueingTime, packetEvent);
    increaseTimeTag<QueueingTimeTag>(packet, queueingTime, queueingTime);
    emit(packetPulledSignal, packet);
    animatePullPacket(packet, outputGate);
    updateDisplayString();
    return packet;
}

void PacketQueue::removePacket(Packet *packet)
{
    Enter_Method("removePacket");
    EV_INFO << "Removing packet" << EV_FIELD(packet) << EV_ENDL;
    queue.remove(packet);
    if (buffer != nullptr)
        buffer->removePacket(packet);
    emit(packetRemovedSignal, packet);
    updateDisplayString();
}

void PacketQueue::removeAllPackets()
{
    Enter_Method("removeAllPackets");
    EV_INFO << "Removing all packets" << EV_ENDL;
    std::vector<Packet *> packets;
    for (int i = 0; i < getNumPackets(); i++)
        packets.push_back(check_and_cast<Packet *>(queue.pop()));
    if (buffer != nullptr)
        buffer->removeAllPackets();
    for (auto packet : packets) {
        emit(packetRemovedSignal, packet);
        delete packet;
    }
    updateDisplayString();
}

bool PacketQueue::canPushSomePacket(cGate *gate) const
{
    if (packetDropperFunction)
        return true;
    if (getMaxNumPackets() != -1 && getNumPackets() >= getMaxNumPackets())
        return false;
    if (getMaxTotalLength() != b(-1) && getTotalLength() >= getMaxTotalLength())
        return false;
    return true;
}

bool PacketQueue::canPushPacket(Packet *packet, cGate *gate) const
{
    if (packetDropperFunction)
        return true;
    if (getMaxNumPackets() != -1 && getNumPackets() >= getMaxNumPackets())
        return false;
    if (getMaxTotalLength() != b(-1) && getMaxTotalLength() - getTotalLength() < packet->getDataLength())
        return false;
    return true;
}

void PacketQueue::handlePacketRemoved(Packet *packet)
{
    Enter_Method("handlePacketRemoved");
    if (queue.contains(packet)) {
        EV_INFO << "Removing packet" << EV_FIELD(packet) << EV_ENDL;
        queue.remove(packet);
        emit(packetRemovedSignal, packet);
        updateDisplayString();
    }
}

/*
 * @sqsq
 */
void PacketQueue::checkAndEmitQueueLoadLevel(Packet *packet)
{
    int queueLoadLevel;
    double occupiedRatio = ((double)getNumPackets()) / packetCapacity;
    if (occupiedRatio < 0.2) {
        queueLoadLevel = 1;
    }
    else if (occupiedRatio < 0.4) {
        queueLoadLevel = 2;
    }
    else if (occupiedRatio < 0.6) {
        queueLoadLevel = 3;
    }
    else if (occupiedRatio < 0.8) {
        queueLoadLevel = 4;
    }
    else {
        queueLoadLevel = 5;
    }


    if (currentQueueLoadLevel != queueLoadLevel) {
        std::cout << "at " << simTime() << " " << this->getParentModule() << std::endl;
        std::cout << getNumPackets() << " " << packetCapacity << std::endl;
        std::cout << "----------------------------------------\n";
        currentQueueLoadLevel = queueLoadLevel;
        QueueLoadLevelDetails details(this->getParentModule(), queueLoadLevel);
        emit(queueLoadLevelSignal, this->getParentModule(), &details);
    }

}

} // namespace queueing
} // namespace inet

