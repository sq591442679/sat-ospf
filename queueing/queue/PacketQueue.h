//
// Copyright (C) 2020 OpenSim Ltd.
//
// SPDX-License-Identifier: LGPL-3.0-or-later
//


#ifndef __INET_PACKETQUEUE_H
#define __INET_PACKETQUEUE_H

#include "inet/queueing/base/PacketQueueBase.h"
#include "inet/queueing/contract/IActivePacketSink.h"
#include "inet/queueing/contract/IActivePacketSource.h"
#include "inet/queueing/contract/IPacketBuffer.h"
#include "inet/queueing/contract/IPacketComparatorFunction.h"
#include "inet/queueing/contract/IPacketDropperFunction.h"

/*
 * @sqsq
 */
#include "inet/networklayer/ipv4/Ipv4.h"
#include <fstream>

namespace inet {
namespace queueing {

/*
 * @sqsq
 */
class INET_API QueueLoadChangeDetails : public cObject
{
private:
    cObject *queuePtr;
    double queueOccupiedRatio;

public:
    QueueLoadChangeDetails(cObject *obj, double ratio):
        queuePtr(obj),
        queueOccupiedRatio(ratio)
    {

    }
    cObject *getQueueptr()
    {
        return queuePtr;
    }
    double getQueueOccupiedRatio()
    {
        return queueOccupiedRatio;
    }
};

class INET_API ELBDetails : public cObject
{
private:
    double chi;
    cObject *module;

public:
    ELBDetails(cObject *obj, double chi):
        module(obj),
        chi(chi)
    {
    }
    double getChi()
    {
        return chi;
    }
    cObject *getIpv4Module()
    {
        return module;
    }
};

class INET_API PacketQueue : public PacketQueueBase, public IPacketBuffer::ICallback
{
  protected:
    int packetCapacity = -1;
    b dataCapacity = b(-1);

    IActivePacketSource *producer = nullptr;
    IActivePacketSink *collector = nullptr;

    cPacketQueue queue;
    IPacketBuffer *buffer = nullptr;

    IPacketDropperFunction *packetDropperFunction = nullptr;
    IPacketComparatorFunction *packetComparatorFunction = nullptr;

    /*
     * @sqsq
     */
    int previousNumPackets = 0;
    static std::ofstream ofs;
    int I = 0; //ELB: monitor queue in every delta interal time
    int O = 0;

  protected:
    virtual void initialize(int stage) override;

    virtual IPacketDropperFunction *createDropperFunction(const char *dropperClass) const;
    virtual IPacketComparatorFunction *createComparatorFunction(const char *comparatorClass) const;

    virtual bool isOverloaded() const;

  public:
    virtual ~PacketQueue(); /*{ delete packetDropperFunction; }*/

    virtual int getMaxNumPackets() const override { return packetCapacity; }
    virtual int getNumPackets() const override;

    virtual b getMaxTotalLength() const override { return dataCapacity; }
    virtual b getTotalLength() const override { return b(queue.getBitLength()); }

    virtual bool isEmpty() const override { return getNumPackets() == 0; }
    virtual Packet *getPacket(int index) const override;
    virtual void removePacket(Packet *packet) override;
    virtual void removeAllPackets() override;

    virtual bool supportsPacketPushing(cGate *gate) const override { return inputGate == gate; }
    virtual bool canPushSomePacket(cGate *gate) const override;
    virtual bool canPushPacket(Packet *packet, cGate *gate) const override;
    virtual void pushPacket(Packet *packet, cGate *gate) override;

    virtual bool supportsPacketPulling(cGate *gate) const override { return outputGate == gate; }
    virtual bool canPullSomePacket(cGate *gate) const override { return !isEmpty(); }
    virtual Packet *canPullPacket(cGate *gate) const override { return !isEmpty() ? getPacket(0) : nullptr; }
    virtual Packet *pullPacket(cGate *gate) override;

    virtual void handlePacketRemoved(Packet *packet) override;

    /*
     * @sqsq
     */
    virtual void checkAndEmitQueueLoadLevel(Packet *packet);
    virtual void finish() override;
    virtual void calculateAndChangeOSPFChi();
};

} // namespace queueing
} // namespace inet

#endif

