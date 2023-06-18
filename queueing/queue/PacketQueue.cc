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
#include "inet/routing/ospfv2/router/Ospfv2Router.h"
#include "inet/routing/ospfv2/router/Ospfv2Area.h"
#include "inet/routing/ospfv2/interface/Ospfv2Interface.h"
#include "inet/routing/ospfv2/Ospfv2.h"
#include "inet/networklayer/contract/ipv4/Ipv4Address.h"
#include "inet/networklayer/common/NetworkInterface.h"
#include "inet/routing/ospfv2/router/Ospfv2Common.h"
#include "inet/networklayer/ipv4/Ipv4.h"
#include <cmath>

namespace inet {
namespace queueing {

Define_Module(PacketQueue);

/*
 * @sqsq
 */
PacketQueue::~PacketQueue()
{
    delete packetDropperFunction;
}

void PacketQueue::finish()
{
    if (ofs.is_open()) {
        ofs.close();
    }
}

std::ofstream PacketQueue::ofs;

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
    if (!ofs.is_open()) {
        std::string filename = "/home/sqsq/Desktop/"
                "sat-ospf/inet/examples/ospfv2/sqsqtest/results/";
        filename += EXPERIMENT_NAME;
        filename += "/";

        filename += IS_OSPF ? "OSPF" : std::to_string(SQSQ_HOP);

        filename += "/";
        filename += getEnvir()->getConfigEx()->getActiveConfigName();
        filename += "/queueDropPacketRaw.csv";
        ofs.open(filename, std::ios::app | std::ios::out);
//        std::cout << "filename: " << filename << "  " << ofs.is_open() << endl;
    }
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

    take(packet);
    cNamedObject packetPushStartedDetails("atomicOperationStarted");
    emit(packetPushStartedSignal, packet, &packetPushStartedDetails);
    EV_INFO << "Pushing packet" << EV_FIELD(packet) << EV_ENDL;
    queue.insert(packet);

    /*
     * @sqsq
     */
//    std::cout << this->getFullName()  << " " << this->getParentModule()->getFullName() << std::endl;
    checkAndEmitQueueLoadLevel(packet);
    I++;

    if (buffer != nullptr)
        buffer->addPacket(packet);
    else if (packetDropperFunction != nullptr) {
        while (isOverloaded()) {
            auto packet = packetDropperFunction->selectPacket(this);

            /*
             * @sqsq
             */
//            std::cout << "at: " << simTime() << " " << this->getParentModule()->getFullPath() << " drop packet" << std::endl;
            if (ofs.is_open() && RECORD_CSV) {
                ofs << getEnvir()->getConfigEx()->getActiveConfigName() << ",";
                ofs << SQSQ_HOP << ",";
                ofs << this->getParentModule()->getFullPath() << ",";
                ofs << simTime() << ",";
                ofs << 0 << ",";
                ofs << 0 << ",";
                ofs << 0 << ",";
                ofs << 1;
                ofs << std::endl;
//                ofs.flush();
            }

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

    EV_INFO << "Pulling packet" << EV_FIELD(packet) << EV_ENDL;
    if (buffer != nullptr) {
        queue.remove(packet);
        buffer->removePacket(packet);
    }
    else
        queue.pop();

    /*
     * @sqsq
     */
    checkAndEmitQueueLoadLevel(packet);
    O++;

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
 * when number of packets changes exceed a threshold (i.e. capacity / 5)
 * emits signal
 */
void PacketQueue::checkAndEmitQueueLoadLevel(Packet *packet)
{
    int currentNumPackets = getNumPackets();
    inet::ospfv2::Ospfv2 *ospfModule = check_and_cast<inet::ospfv2::Ospfv2 *>(this->getParentModule()->getParentModule()->getSubmodule("ospf"));
    ospfv2::Router *ospfRouter = ospfModule->getOspfRouter();
    ospfv2::Ospfv2Area *ospfArea = ospfRouter->getAreaByID(Ipv4Address(0, 0, 0, 0));
    Ipv4Address routerID = ospfRouter->getRouterID();
    NetworkInterface *currentInterface = check_and_cast<NetworkInterface *>(this->getParentModule());
    double totalPropagationDelay = 0.0;
    double maxPropagationDelay = 0.0;
    std::vector<int> interfaceIndices = ospfArea->getInterfaceIndices();

    if (ospfv2::sqsqCheckSimTime() && LOAD_BALANCE) {
        for (int index : interfaceIndices) {
            ospfv2::Ospfv2Interface *associatedInterface = ospfArea->getInterface(index);
            std::string associatedInterfaceName = associatedInterface->getInterfaceName();
            std::string currentInterfaceName = currentInterface->getInterfaceName();
            if (associatedInterface->getState() ==
                    ospfv2::Ospfv2Interface::Ospfv2InterfaceStateType::POINTTOPOINT_STATE
                &&
                    associatedInterfaceName != currentInterfaceName) { // 找到该路由器在工作状态下的其它接口
                int direction = associatedInterfaceName[associatedInterfaceName.length() - 1] - '0';
                if (direction == 0 || direction == 1) {
                    maxPropagationDelay = std::max(maxPropagationDelay, ospfv2::propagationDelayByID.find(0)->second);
                    totalPropagationDelay += ospfv2::propagationDelayByID.find(0)->second;
                }
                else {
                    maxPropagationDelay = std::max(maxPropagationDelay, ospfv2::propagationDelayByID.find(2)->second);
                    totalPropagationDelay += ospfv2::propagationDelayByID.find(routerID.getDByte(2))->second;
                }
            }
        }
        int reservedPacketThreshold = std::ceil(totalPropagationDelay * (double)ospfv2::bandwidth / ospfv2::averagePacketSize);

        if (std::abs(currentNumPackets - previousNumPackets) >= getMaxNumPackets() * LOAD_SCALE) { // 在"普通状态下"的队列占用波动
            double queueOccupiedRatio = ((double)(currentNumPackets)) / (double)getMaxNumPackets();
            previousNumPackets = currentNumPackets;

            QueueLoadChangeDetails details(this->getParentModule(), queueOccupiedRatio);
            emit(queueLoadLevelSignal, this->getParentModule(), &details);
        }
    }
}

/*
 * @sqsq
 * 计算该队列对应的chi并更新ospf中的chiArray
 */
void PacketQueue::calculateAndChangeOSPFChi()
{
    int currentNumPackets = getNumPackets();
    inet::ospfv2::Ospfv2 *ospfModule = check_and_cast<inet::ospfv2::Ospfv2 *>(this->getParentModule()->getParentModule()->getSubmodule("ospf"));
    ospfv2::Router *ospfRouter = ospfModule->getOspfRouter();
    ospfv2::Ospfv2Area *ospfArea = ospfRouter->getAreaByID(Ipv4Address(0, 0, 0, 0));
    Ipv4Address routerID = ospfRouter->getRouterID();
    NetworkInterface *currentInterface = check_and_cast<NetworkInterface *>(this->getParentModule());
    double totalPropagationDelay = 0.0;
    double maxPropagationDelay = 0.0;
    std::vector<int> interfaceIndices = ospfArea->getInterfaceIndices();

    for (int index : interfaceIndices) {
        ospfv2::Ospfv2Interface *associatedInterface = ospfArea->getInterface(index);
        std::string associatedInterfaceName = associatedInterface->getInterfaceName();
        std::string currentInterfaceName = currentInterface->getInterfaceName();
        if (associatedInterface->getState() ==
                ospfv2::Ospfv2Interface::Ospfv2InterfaceStateType::POINTTOPOINT_STATE
            &&
                associatedInterfaceName != currentInterfaceName) { // 找到该路由器在工作状态下的其它接口
            int direction = associatedInterfaceName[associatedInterfaceName.length() - 1] - '0';
            if (direction == 0 || direction == 1) {
                maxPropagationDelay = std::max(maxPropagationDelay, ospfv2::propagationDelayByID.find(0)->second);
                totalPropagationDelay += ospfv2::propagationDelayByID.find(0)->second;
            }
            else {
                maxPropagationDelay = std::max(maxPropagationDelay, ospfv2::propagationDelayByID.find(2)->second);
                totalPropagationDelay += ospfv2::propagationDelayByID.find(routerID.getDByte(2))->second;
            }
        }
    }

    double delta = ospfModule->getDelta();
    if (I - O != 0) {
        double elapsingTime = (double)(getMaxNumPackets() - getNumPackets())
                / (double)(I - O);
        double p = std::min(1.0, (delta + maxPropagationDelay) / elapsingTime);
        double beta = 1.0 - p, alpha = beta / 2;
        double qtBSA = std::min(
                (double)getMaxNumPackets() * beta + totalPropagationDelay * (I - O),
                (double)getMaxNumPackets());
        double theta = 10 * totalPropagationDelay;
        double INew = (qtBSA - getMaxNumPackets() * alpha) / theta + O;
        double chi;
        if ((double)getNumPackets() >= (double)getMaxNumPackets() * beta) {
            chi = std::min(std::max(0.0, INew / I), 1.0);
        }
        else {
            chi = 0.0;
        }

        const NetworkInterface *ie = check_and_cast<const NetworkInterface *>(this->getParentModule());
        std::string interfaceName = ie->getInterfaceName();
        int direction = interfaceName[interfaceName.length() - 1] - '0';
        ospfModule->getChiArray()[direction] = chi;
//        std::cout << "at " << simTime() << " " << this->getFullPath() <<
//                " changes chi to " << chi << " Inew:" << INew << " I:" << I << std::endl;
//            ELBDetails details(this->getParentModule(), chi);
//            emit(ELBChiSignal, this->getParentModule(), &details);
    }
    I = 0;
    O = 0;
}
} // namespace queueing
} // namespace inet

