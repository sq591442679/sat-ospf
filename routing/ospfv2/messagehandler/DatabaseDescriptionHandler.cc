//
// Copyright (C) 2006 Andras Babos and Andras Varga
//
// SPDX-License-Identifier: LGPL-3.0-or-later
//

#include "inet/routing/ospfv2/messagehandler/DatabaseDescriptionHandler.h"

#include "inet/routing/ospfv2/interface/Ospfv2Interface.h"
#include "inet/routing/ospfv2/neighbor/Ospfv2Neighbor.h"
#include "inet/routing/ospfv2/router/Ospfv2Area.h"
#include "inet/routing/ospfv2/router/Ospfv2Router.h"

#include "inet/routing/ospfv2/router/Ospfv2Common.h"
#include "inet/networklayer/contract/ipv4/Ipv4Address.h"

namespace inet {
namespace ospfv2 {

DatabaseDescriptionHandler::DatabaseDescriptionHandler(Router *containingRouter) :
    IMessageHandler(containingRouter)
{
}

void DatabaseDescriptionHandler::processPacket(Packet *packet, Ospfv2Interface *intf, Neighbor *neighbor)
{
    router->getMessageHandler()->printEvent("Database Description packet received", intf, neighbor);

    const auto& ddPacket = packet->peekAtFront<Ospfv2DatabaseDescriptionPacket>();

    Neighbor::NeighborStateType neighborState = neighbor->getState();

    if ((ddPacket->getInterfaceMTU() <= intf->getMtu()) &&
        (neighborState > Neighbor::ATTEMPT_STATE))
    {
        switch (neighborState) {
            case Neighbor::TWOWAY_STATE:
                break;

            case Neighbor::INIT_STATE:
                neighbor->processEvent(Neighbor::TWOWAY_RECEIVED);
                break;

            case Neighbor::EXCHANGE_START_STATE: {
                const Ospfv2DdOptions& ddOptions = ddPacket->getDdOptions();

                if (ddOptions.I_Init && ddOptions.M_More && ddOptions.MS_MasterSlave &&
                    (ddPacket->getLsaHeadersArraySize() == 0))
                {
                    if (neighbor->getNeighborID() > router->getRouterID()) {
                        Neighbor::DdPacketId packetID;
                        packetID.ddOptions = ddOptions;
                        packetID.options = ddPacket->getOptions();
                        packetID.sequenceNumber = ddPacket->getDdSequenceNumber();

                        neighbor->setOptions(packetID.options);
                        neighbor->setDatabaseExchangeRelationship(Neighbor::SLAVE);
                        neighbor->setDDSequenceNumber(packetID.sequenceNumber);
                        neighbor->setLastReceivedDDPacket(packetID);

                        if (!processDDPacket(ddPacket.get(), intf, neighbor, true)) {
                            break;
                        }

                        neighbor->processEvent(Neighbor::NEGOTIATION_DONE);
                        if (!neighbor->isLinkStateRequestListEmpty() &&
                            !neighbor->isRequestRetransmissionTimerActive())
                        {
                            neighbor->sendLinkStateRequestPacket();
                            neighbor->clearRequestRetransmissionTimer();
                            neighbor->startRequestRetransmissionTimer();
                        }
                    }
                    else {
                        neighbor->sendDatabaseDescriptionPacket(true);
                    }
                }
                if (!ddOptions.I_Init && !ddOptions.MS_MasterSlave &&
                    (ddPacket->getDdSequenceNumber() == neighbor->getDDSequenceNumber()) &&
                    (neighbor->getNeighborID() < router->getRouterID()))
                {
                    Neighbor::DdPacketId packetID;
                    packetID.ddOptions = ddOptions;
                    packetID.options = ddPacket->getOptions();
                    packetID.sequenceNumber = ddPacket->getDdSequenceNumber();

                    neighbor->setOptions(packetID.options);
                    neighbor->setDatabaseExchangeRelationship(Neighbor::MASTER);
                    neighbor->setLastReceivedDDPacket(packetID);

                    if (!processDDPacket(ddPacket.get(), intf, neighbor, true)) {
                        break;
                    }

                    neighbor->processEvent(Neighbor::NEGOTIATION_DONE);
                    if (!neighbor->isLinkStateRequestListEmpty() &&
                        !neighbor->isRequestRetransmissionTimerActive())
                    {
                        neighbor->sendLinkStateRequestPacket();
                        neighbor->clearRequestRetransmissionTimer();
                        neighbor->startRequestRetransmissionTimer();
                    }
                }
            }
            break;

            case Neighbor::EXCHANGE_STATE: {
                Neighbor::DdPacketId packetID;
                packetID.ddOptions = ddPacket->getDdOptions();
                packetID.options = ddPacket->getOptions();
                packetID.sequenceNumber = ddPacket->getDdSequenceNumber();

                if (packetID != neighbor->getLastReceivedDDPacket()) {
                    if ((packetID.ddOptions.MS_MasterSlave &&
                         (neighbor->getDatabaseExchangeRelationship() != Neighbor::SLAVE)) ||
                        (!packetID.ddOptions.MS_MasterSlave &&
                         (neighbor->getDatabaseExchangeRelationship() != Neighbor::MASTER)) ||
                        packetID.ddOptions.I_Init ||
                        (packetID.options != neighbor->getLastReceivedDDPacket().options))
                    {
                        neighbor->processEvent(Neighbor::SEQUENCE_NUMBER_MISMATCH);
                    }
                    else {
                        if (((neighbor->getDatabaseExchangeRelationship() == Neighbor::MASTER) &&
                             (packetID.sequenceNumber == neighbor->getDDSequenceNumber())) ||
                            ((neighbor->getDatabaseExchangeRelationship() == Neighbor::SLAVE) &&
                             (packetID.sequenceNumber == (neighbor->getDDSequenceNumber() + 1))))
                        {
                            neighbor->setLastReceivedDDPacket(packetID);
                            if (!processDDPacket(ddPacket.get(), intf, neighbor, false)) {
                                break;
                            }
                            if (!neighbor->isLinkStateRequestListEmpty() &&
                                !neighbor->isRequestRetransmissionTimerActive())
                            {
                                neighbor->sendLinkStateRequestPacket();
                                neighbor->clearRequestRetransmissionTimer();
                                neighbor->startRequestRetransmissionTimer();
                            }
                        }
                        else {
                            neighbor->processEvent(Neighbor::SEQUENCE_NUMBER_MISMATCH);
                        }
                    }
                }
                else {
                    if (neighbor->getDatabaseExchangeRelationship() == Neighbor::SLAVE) {
                        neighbor->retransmitDatabaseDescriptionPacket();
                    }
                }
            }
            break;

            case Neighbor::LOADING_STATE:
            case Neighbor::FULL_STATE: {
                Neighbor::DdPacketId packetID;
                packetID.ddOptions = ddPacket->getDdOptions();
                packetID.options = ddPacket->getOptions();
                packetID.sequenceNumber = ddPacket->getDdSequenceNumber();

                if ((packetID != neighbor->getLastReceivedDDPacket()) ||
                    (packetID.ddOptions.I_Init))
                {
                    neighbor->processEvent(Neighbor::SEQUENCE_NUMBER_MISMATCH);
                }
                else {
                    if (neighbor->getDatabaseExchangeRelationship() == Neighbor::SLAVE) {
                        if (!neighbor->retransmitDatabaseDescriptionPacket()) {
                            neighbor->processEvent(Neighbor::SEQUENCE_NUMBER_MISMATCH);
                        }
                    }
                }
            }
            break;

            default:
                break;
        }
    }
}

bool DatabaseDescriptionHandler::processDDPacket(const Ospfv2DatabaseDescriptionPacket *ddPacket, Ospfv2Interface *intf, Neighbor *neighbor, bool inExchangeStart)
{
    EV_INFO << "  Processing packet contents(ddOptions="
            << ((ddPacket->getDdOptions().I_Init) ? "I " : "_ ")
            << ((ddPacket->getDdOptions().M_More) ? "M " : "_ ")
            << ((ddPacket->getDdOptions().MS_MasterSlave) ? "MS" : "__")
            << "; seqNumber="
            << ddPacket->getDdSequenceNumber()
            << "):\n";

    unsigned int headerCount = ddPacket->getLsaHeadersArraySize();

    for (unsigned int i = 0; i < headerCount; i++) {
        const Ospfv2LsaHeader& currentHeader = ddPacket->getLsaHeaders(i);
        Ospfv2LsaType lsaType = static_cast<Ospfv2LsaType>(currentHeader.getLsType());

//        EV_DETAIL << "    " << currentHeader;

        if ((lsaType < ROUTERLSA_TYPE) || (lsaType > AS_EXTERNAL_LSA_TYPE) ||
            ((lsaType == AS_EXTERNAL_LSA_TYPE) && (!intf->getArea()->getExternalRoutingCapability())))
        {
            EV_ERROR << " Error!\n";
            neighbor->processEvent(Neighbor::SEQUENCE_NUMBER_MISMATCH);
            return false;
        }
        else {
            LsaKeyType lsaKey;

            lsaKey.linkStateID = currentHeader.getLinkStateID();
            lsaKey.advertisingRouter = currentHeader.getAdvertisingRouter();

            Ospfv2Lsa *lsaInDatabase = router->findLSA(lsaType, lsaKey, intf->getArea()->getAreaID());

            // operator< and operator== on OSPFLSAHeaders determines which one is newer(less means older)
            if ((lsaInDatabase == nullptr) || (lsaInDatabase->getHeader() < currentHeader)) {
                EV_DETAIL << " (newer)";
                /*
                 * @sqsq
                 */
                if (sqsqCheckSimTime()) {
                    if (REQUEST_SHOULD_KNOWN_RANGE) {
                        // @sqsq: only request the LSA which is in current router's "ought-to-know" area
                        // @sqsq： defination of linkStateID is in rfc 2328 table 16
                        Ipv4Address currentRouterID = router->getRouterID();
                        Ipv4Address linkStateID = currentHeader.getLinkStateID();
                        int dis = 0x3f3f3f3f;
                        if (currentHeader.getLsType() == ROUTERLSA_TYPE) {
                            dis = sqsqCalculateManhattanDistance(currentRouterID, linkStateID);
                            if (dis <= LSR_RANGE) {
                                neighbor->addToRequestList(&currentHeader);
    //                            std::cout << "at " << simTime() << ", " << currentRouterID << ": " << linkStateID << " dis=" << dis << std::endl;
                            }
                        }
                        else if (currentHeader.getLsType() == NETWORKLSA_TYPE) {
                            Ipv4Address networkAddress;
                            networkAddress.set(linkStateID.getDByte(0), linkStateID.getDByte(1), linkStateID.getDByte(2), 0);
                            Ipv4Address end1 = routerIDsByNetwork.find(networkAddress)->second.first;
                            Ipv4Address end2 = routerIDsByNetwork.find(networkAddress)->second.second; // 2 router ids that link to this network
                            int dis1 = sqsqCalculateManhattanDistance(currentRouterID, end1);
                            int dis2 = sqsqCalculateManhattanDistance(currentRouterID, end2);
                            dis = std::min(dis1, dis2);
                            if (dis <= LSR_RANGE) {
                                neighbor->addToRequestList(&currentHeader);
    //                            std::cout << "at " << simTime() << ", " << currentRouterID << ": " << linkStateID << " dis=" << dis << std::endl;
                            }
                        }
    //                    std::cout << currentRouterID << ": " << linkStateID << " dis=" << dis << std::endl;
                    }
                }
                else {
                    neighbor->addToRequestList(&currentHeader);
                }
            }
        }
        EV_DETAIL << "\n";
    }

    if (neighbor->getDatabaseExchangeRelationship() == Neighbor::MASTER) {
        neighbor->incrementDDSequenceNumber();
        if ((neighbor->getDatabaseSummaryListCount() == 0) && !ddPacket->getDdOptions().M_More) {
            neighbor->processEvent(Neighbor::EXCHANGE_DONE); // does nothing in ExchangeStart
        }
        else {
            if (!inExchangeStart) {
                neighbor->sendDatabaseDescriptionPacket();
            }
        }
    }
    else {
        neighbor->setDDSequenceNumber(ddPacket->getDdSequenceNumber());
        if (!inExchangeStart) {
            neighbor->sendDatabaseDescriptionPacket();
        }
        if (!ddPacket->getDdOptions().M_More &&
            (neighbor->getDatabaseSummaryListCount() == 0))
        {
            neighbor->processEvent(Neighbor::EXCHANGE_DONE); // does nothing in ExchangeStart
        }
    }
    return true;
}

} // namespace ospfv2
} // namespace inet

