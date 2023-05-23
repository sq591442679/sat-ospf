//
// Copyright (C) 2006 Andras Babos and Andras Varga
//
// SPDX-License-Identifier: LGPL-3.0-or-later
//

#ifndef __INET_OSPFV2COMMON_H
#define __INET_OSPFV2COMMON_H

#include <ctype.h>
#include <stdio.h>

#include <stdlib.h>

#include <functional>

#include "inet/common/Units_m.h"
#include "inet/networklayer/common/L3AddressResolver.h"
#include "inet/networklayer/contract/ipv4/Ipv4Address.h"

#include <map>
#include <utility>
#include <vector>

namespace inet {

namespace ospfv2 {

// global constants
#define LS_REFRESH_TIME                        1800
#define MIN_LS_INTERVAL                        5
#define MIN_LS_ARRIVAL                         1
#define MAX_AGE                                3600
#define CHECK_AGE                              300
#define MAX_AGE_DIFF                           900
#define LS_INFINITY                            16777215
#define DEFAULT_DESTINATION_ADDRESS            0
#define DEFAULT_DESTINATION_MASK               0
#define INITIAL_SEQUENCE_NUMBER                -2147483647
#define MAX_SEQUENCE_NUMBER                    2147483647

#define VIRTUAL_LINK_TTL                       32

/*
 * @sqsq
 */
#define SQSQ_CONVERGENCY_TIME                  20.000000
#define SQSQ_HOP                               0
#define EXPERIMENT_NAME                        "withDD-withLoopPrevention-withLoadBalance"

#define SQSQ_M                                 6    // num of orbits
#define SQSQ_N                                 11   // num of satellites in one orbit

#define PRINT_FULL_DURATION                    false
#define PRINT_IVP4_DROP_PACKET                 false

#define LOOP_AVOIDANCE                         true
#define HOP_LOOP_PARAMETER                     10

#define REQUEST_SHOULD_KNOWN_RANGE             true
#define LSR_RANGE                              SQSQ_HOP

#define LOAD_BALANCE                           true

const std::map<Ipv4Address, std::pair<Ipv4Address, Ipv4Address> > routerIDsByNetwork = {
        {Ipv4Address(192, 168, 1, 0), std::make_pair(Ipv4Address(0, 0, 1, 1), Ipv4Address(0, 0, 1, 2))},
        {Ipv4Address(192, 168, 2, 0), std::make_pair(Ipv4Address(0, 0, 1, 1), Ipv4Address(0, 0, 2, 1))},
        {Ipv4Address(192, 168, 3, 0), std::make_pair(Ipv4Address(0, 0, 1, 2), Ipv4Address(0, 0, 1, 3))},
        {Ipv4Address(192, 168, 4, 0), std::make_pair(Ipv4Address(0, 0, 1, 2), Ipv4Address(0, 0, 2, 2))},
        {Ipv4Address(192, 168, 5, 0), std::make_pair(Ipv4Address(0, 0, 1, 3), Ipv4Address(0, 0, 1, 4))},
        {Ipv4Address(192, 168, 6, 0), std::make_pair(Ipv4Address(0, 0, 1, 3), Ipv4Address(0, 0, 2, 3))},
        {Ipv4Address(192, 168, 7, 0), std::make_pair(Ipv4Address(0, 0, 1, 4), Ipv4Address(0, 0, 1, 5))},
        {Ipv4Address(192, 168, 8, 0), std::make_pair(Ipv4Address(0, 0, 1, 4), Ipv4Address(0, 0, 2, 4))},
        {Ipv4Address(192, 168, 9, 0), std::make_pair(Ipv4Address(0, 0, 1, 5), Ipv4Address(0, 0, 1, 6))},
        {Ipv4Address(192, 168, 10, 0), std::make_pair(Ipv4Address(0, 0, 1, 5), Ipv4Address(0, 0, 2, 5))},
        {Ipv4Address(192, 168, 11, 0), std::make_pair(Ipv4Address(0, 0, 1, 6), Ipv4Address(0, 0, 1, 1))},
        {Ipv4Address(192, 168, 12, 0), std::make_pair(Ipv4Address(0, 0, 1, 6), Ipv4Address(0, 0, 2, 6))},
        {Ipv4Address(192, 168, 13, 0), std::make_pair(Ipv4Address(0, 0, 2, 1), Ipv4Address(0, 0, 2, 2))},
        {Ipv4Address(192, 168, 14, 0), std::make_pair(Ipv4Address(0, 0, 2, 1), Ipv4Address(0, 0, 3, 1))},
        {Ipv4Address(192, 168, 15, 0), std::make_pair(Ipv4Address(0, 0, 2, 2), Ipv4Address(0, 0, 2, 3))},
        {Ipv4Address(192, 168, 16, 0), std::make_pair(Ipv4Address(0, 0, 2, 2), Ipv4Address(0, 0, 3, 2))},
        {Ipv4Address(192, 168, 17, 0), std::make_pair(Ipv4Address(0, 0, 2, 3), Ipv4Address(0, 0, 2, 4))},
        {Ipv4Address(192, 168, 18, 0), std::make_pair(Ipv4Address(0, 0, 2, 3), Ipv4Address(0, 0, 3, 3))},
        {Ipv4Address(192, 168, 19, 0), std::make_pair(Ipv4Address(0, 0, 2, 4), Ipv4Address(0, 0, 2, 5))},
        {Ipv4Address(192, 168, 20, 0), std::make_pair(Ipv4Address(0, 0, 2, 4), Ipv4Address(0, 0, 3, 4))},
        {Ipv4Address(192, 168, 21, 0), std::make_pair(Ipv4Address(0, 0, 2, 5), Ipv4Address(0, 0, 2, 6))},
        {Ipv4Address(192, 168, 22, 0), std::make_pair(Ipv4Address(0, 0, 2, 5), Ipv4Address(0, 0, 3, 5))},
        {Ipv4Address(192, 168, 23, 0), std::make_pair(Ipv4Address(0, 0, 2, 6), Ipv4Address(0, 0, 2, 1))},
        {Ipv4Address(192, 168, 24, 0), std::make_pair(Ipv4Address(0, 0, 2, 6), Ipv4Address(0, 0, 3, 6))},
        {Ipv4Address(192, 168, 25, 0), std::make_pair(Ipv4Address(0, 0, 3, 1), Ipv4Address(0, 0, 3, 2))},
        {Ipv4Address(192, 168, 26, 0), std::make_pair(Ipv4Address(0, 0, 3, 1), Ipv4Address(0, 0, 4, 1))},
        {Ipv4Address(192, 168, 27, 0), std::make_pair(Ipv4Address(0, 0, 3, 2), Ipv4Address(0, 0, 3, 3))},
        {Ipv4Address(192, 168, 28, 0), std::make_pair(Ipv4Address(0, 0, 3, 2), Ipv4Address(0, 0, 4, 2))},
        {Ipv4Address(192, 168, 29, 0), std::make_pair(Ipv4Address(0, 0, 3, 3), Ipv4Address(0, 0, 3, 4))},
        {Ipv4Address(192, 168, 30, 0), std::make_pair(Ipv4Address(0, 0, 3, 3), Ipv4Address(0, 0, 4, 3))},
        {Ipv4Address(192, 168, 31, 0), std::make_pair(Ipv4Address(0, 0, 3, 4), Ipv4Address(0, 0, 3, 5))},
        {Ipv4Address(192, 168, 32, 0), std::make_pair(Ipv4Address(0, 0, 3, 4), Ipv4Address(0, 0, 4, 4))},
        {Ipv4Address(192, 168, 33, 0), std::make_pair(Ipv4Address(0, 0, 3, 5), Ipv4Address(0, 0, 3, 6))},
        {Ipv4Address(192, 168, 34, 0), std::make_pair(Ipv4Address(0, 0, 3, 5), Ipv4Address(0, 0, 4, 5))},
        {Ipv4Address(192, 168, 35, 0), std::make_pair(Ipv4Address(0, 0, 3, 6), Ipv4Address(0, 0, 3, 1))},
        {Ipv4Address(192, 168, 36, 0), std::make_pair(Ipv4Address(0, 0, 3, 6), Ipv4Address(0, 0, 4, 6))},
        {Ipv4Address(192, 168, 37, 0), std::make_pair(Ipv4Address(0, 0, 4, 1), Ipv4Address(0, 0, 4, 2))},
        {Ipv4Address(192, 168, 38, 0), std::make_pair(Ipv4Address(0, 0, 4, 1), Ipv4Address(0, 0, 5, 1))},
        {Ipv4Address(192, 168, 39, 0), std::make_pair(Ipv4Address(0, 0, 4, 2), Ipv4Address(0, 0, 4, 3))},
        {Ipv4Address(192, 168, 40, 0), std::make_pair(Ipv4Address(0, 0, 4, 2), Ipv4Address(0, 0, 5, 2))},
        {Ipv4Address(192, 168, 41, 0), std::make_pair(Ipv4Address(0, 0, 4, 3), Ipv4Address(0, 0, 4, 4))},
        {Ipv4Address(192, 168, 42, 0), std::make_pair(Ipv4Address(0, 0, 4, 3), Ipv4Address(0, 0, 5, 3))},
        {Ipv4Address(192, 168, 43, 0), std::make_pair(Ipv4Address(0, 0, 4, 4), Ipv4Address(0, 0, 4, 5))},
        {Ipv4Address(192, 168, 44, 0), std::make_pair(Ipv4Address(0, 0, 4, 4), Ipv4Address(0, 0, 5, 4))},
        {Ipv4Address(192, 168, 45, 0), std::make_pair(Ipv4Address(0, 0, 4, 5), Ipv4Address(0, 0, 4, 6))},
        {Ipv4Address(192, 168, 46, 0), std::make_pair(Ipv4Address(0, 0, 4, 5), Ipv4Address(0, 0, 5, 5))},
        {Ipv4Address(192, 168, 47, 0), std::make_pair(Ipv4Address(0, 0, 4, 6), Ipv4Address(0, 0, 4, 1))},
        {Ipv4Address(192, 168, 48, 0), std::make_pair(Ipv4Address(0, 0, 4, 6), Ipv4Address(0, 0, 5, 6))},
        {Ipv4Address(192, 168, 49, 0), std::make_pair(Ipv4Address(0, 0, 5, 1), Ipv4Address(0, 0, 5, 2))},
        {Ipv4Address(192, 168, 50, 0), std::make_pair(Ipv4Address(0, 0, 5, 1), Ipv4Address(0, 0, 6, 1))},
        {Ipv4Address(192, 168, 51, 0), std::make_pair(Ipv4Address(0, 0, 5, 2), Ipv4Address(0, 0, 5, 3))},
        {Ipv4Address(192, 168, 52, 0), std::make_pair(Ipv4Address(0, 0, 5, 2), Ipv4Address(0, 0, 6, 2))},
        {Ipv4Address(192, 168, 53, 0), std::make_pair(Ipv4Address(0, 0, 5, 3), Ipv4Address(0, 0, 5, 4))},
        {Ipv4Address(192, 168, 54, 0), std::make_pair(Ipv4Address(0, 0, 5, 3), Ipv4Address(0, 0, 6, 3))},
        {Ipv4Address(192, 168, 55, 0), std::make_pair(Ipv4Address(0, 0, 5, 4), Ipv4Address(0, 0, 5, 5))},
        {Ipv4Address(192, 168, 56, 0), std::make_pair(Ipv4Address(0, 0, 5, 4), Ipv4Address(0, 0, 6, 4))},
        {Ipv4Address(192, 168, 57, 0), std::make_pair(Ipv4Address(0, 0, 5, 5), Ipv4Address(0, 0, 5, 6))},
        {Ipv4Address(192, 168, 58, 0), std::make_pair(Ipv4Address(0, 0, 5, 5), Ipv4Address(0, 0, 6, 5))},
        {Ipv4Address(192, 168, 59, 0), std::make_pair(Ipv4Address(0, 0, 5, 6), Ipv4Address(0, 0, 5, 1))},
        {Ipv4Address(192, 168, 60, 0), std::make_pair(Ipv4Address(0, 0, 5, 6), Ipv4Address(0, 0, 6, 6))},
        {Ipv4Address(192, 168, 61, 0), std::make_pair(Ipv4Address(0, 0, 6, 1), Ipv4Address(0, 0, 6, 2))},
        {Ipv4Address(192, 168, 62, 0), std::make_pair(Ipv4Address(0, 0, 6, 1), Ipv4Address(0, 0, 7, 1))},
        {Ipv4Address(192, 168, 63, 0), std::make_pair(Ipv4Address(0, 0, 6, 2), Ipv4Address(0, 0, 6, 3))},
        {Ipv4Address(192, 168, 64, 0), std::make_pair(Ipv4Address(0, 0, 6, 2), Ipv4Address(0, 0, 7, 2))},
        {Ipv4Address(192, 168, 65, 0), std::make_pair(Ipv4Address(0, 0, 6, 3), Ipv4Address(0, 0, 6, 4))},
        {Ipv4Address(192, 168, 66, 0), std::make_pair(Ipv4Address(0, 0, 6, 3), Ipv4Address(0, 0, 7, 3))},
        {Ipv4Address(192, 168, 67, 0), std::make_pair(Ipv4Address(0, 0, 6, 4), Ipv4Address(0, 0, 6, 5))},
        {Ipv4Address(192, 168, 68, 0), std::make_pair(Ipv4Address(0, 0, 6, 4), Ipv4Address(0, 0, 7, 4))},
        {Ipv4Address(192, 168, 69, 0), std::make_pair(Ipv4Address(0, 0, 6, 5), Ipv4Address(0, 0, 6, 6))},
        {Ipv4Address(192, 168, 70, 0), std::make_pair(Ipv4Address(0, 0, 6, 5), Ipv4Address(0, 0, 7, 5))},
        {Ipv4Address(192, 168, 71, 0), std::make_pair(Ipv4Address(0, 0, 6, 6), Ipv4Address(0, 0, 6, 1))},
        {Ipv4Address(192, 168, 72, 0), std::make_pair(Ipv4Address(0, 0, 6, 6), Ipv4Address(0, 0, 7, 6))},
        {Ipv4Address(192, 168, 73, 0), std::make_pair(Ipv4Address(0, 0, 7, 1), Ipv4Address(0, 0, 7, 2))},
        {Ipv4Address(192, 168, 74, 0), std::make_pair(Ipv4Address(0, 0, 7, 1), Ipv4Address(0, 0, 8, 1))},
        {Ipv4Address(192, 168, 75, 0), std::make_pair(Ipv4Address(0, 0, 7, 2), Ipv4Address(0, 0, 7, 3))},
        {Ipv4Address(192, 168, 76, 0), std::make_pair(Ipv4Address(0, 0, 7, 2), Ipv4Address(0, 0, 8, 2))},
        {Ipv4Address(192, 168, 77, 0), std::make_pair(Ipv4Address(0, 0, 7, 3), Ipv4Address(0, 0, 7, 4))},
        {Ipv4Address(192, 168, 78, 0), std::make_pair(Ipv4Address(0, 0, 7, 3), Ipv4Address(0, 0, 8, 3))},
        {Ipv4Address(192, 168, 79, 0), std::make_pair(Ipv4Address(0, 0, 7, 4), Ipv4Address(0, 0, 7, 5))},
        {Ipv4Address(192, 168, 80, 0), std::make_pair(Ipv4Address(0, 0, 7, 4), Ipv4Address(0, 0, 8, 4))},
        {Ipv4Address(192, 168, 81, 0), std::make_pair(Ipv4Address(0, 0, 7, 5), Ipv4Address(0, 0, 7, 6))},
        {Ipv4Address(192, 168, 82, 0), std::make_pair(Ipv4Address(0, 0, 7, 5), Ipv4Address(0, 0, 8, 5))},
        {Ipv4Address(192, 168, 83, 0), std::make_pair(Ipv4Address(0, 0, 7, 6), Ipv4Address(0, 0, 7, 1))},
        {Ipv4Address(192, 168, 84, 0), std::make_pair(Ipv4Address(0, 0, 7, 6), Ipv4Address(0, 0, 8, 6))},
        {Ipv4Address(192, 168, 85, 0), std::make_pair(Ipv4Address(0, 0, 8, 1), Ipv4Address(0, 0, 8, 2))},
        {Ipv4Address(192, 168, 86, 0), std::make_pair(Ipv4Address(0, 0, 8, 1), Ipv4Address(0, 0, 9, 1))},
        {Ipv4Address(192, 168, 87, 0), std::make_pair(Ipv4Address(0, 0, 8, 2), Ipv4Address(0, 0, 8, 3))},
        {Ipv4Address(192, 168, 88, 0), std::make_pair(Ipv4Address(0, 0, 8, 2), Ipv4Address(0, 0, 9, 2))},
        {Ipv4Address(192, 168, 89, 0), std::make_pair(Ipv4Address(0, 0, 8, 3), Ipv4Address(0, 0, 8, 4))},
        {Ipv4Address(192, 168, 90, 0), std::make_pair(Ipv4Address(0, 0, 8, 3), Ipv4Address(0, 0, 9, 3))},
        {Ipv4Address(192, 168, 91, 0), std::make_pair(Ipv4Address(0, 0, 8, 4), Ipv4Address(0, 0, 8, 5))},
        {Ipv4Address(192, 168, 92, 0), std::make_pair(Ipv4Address(0, 0, 8, 4), Ipv4Address(0, 0, 9, 4))},
        {Ipv4Address(192, 168, 93, 0), std::make_pair(Ipv4Address(0, 0, 8, 5), Ipv4Address(0, 0, 8, 6))},
        {Ipv4Address(192, 168, 94, 0), std::make_pair(Ipv4Address(0, 0, 8, 5), Ipv4Address(0, 0, 9, 5))},
        {Ipv4Address(192, 168, 95, 0), std::make_pair(Ipv4Address(0, 0, 8, 6), Ipv4Address(0, 0, 8, 1))},
        {Ipv4Address(192, 168, 96, 0), std::make_pair(Ipv4Address(0, 0, 8, 6), Ipv4Address(0, 0, 9, 6))},
        {Ipv4Address(192, 168, 97, 0), std::make_pair(Ipv4Address(0, 0, 9, 1), Ipv4Address(0, 0, 9, 2))},
        {Ipv4Address(192, 168, 98, 0), std::make_pair(Ipv4Address(0, 0, 9, 1), Ipv4Address(0, 0, 10, 1))},
        {Ipv4Address(192, 168, 99, 0), std::make_pair(Ipv4Address(0, 0, 9, 2), Ipv4Address(0, 0, 9, 3))},
        {Ipv4Address(192, 168, 100, 0), std::make_pair(Ipv4Address(0, 0, 9, 2), Ipv4Address(0, 0, 10, 2))},
        {Ipv4Address(192, 168, 101, 0), std::make_pair(Ipv4Address(0, 0, 9, 3), Ipv4Address(0, 0, 9, 4))},
        {Ipv4Address(192, 168, 102, 0), std::make_pair(Ipv4Address(0, 0, 9, 3), Ipv4Address(0, 0, 10, 3))},
        {Ipv4Address(192, 168, 103, 0), std::make_pair(Ipv4Address(0, 0, 9, 4), Ipv4Address(0, 0, 9, 5))},
        {Ipv4Address(192, 168, 104, 0), std::make_pair(Ipv4Address(0, 0, 9, 4), Ipv4Address(0, 0, 10, 4))},
        {Ipv4Address(192, 168, 105, 0), std::make_pair(Ipv4Address(0, 0, 9, 5), Ipv4Address(0, 0, 9, 6))},
        {Ipv4Address(192, 168, 106, 0), std::make_pair(Ipv4Address(0, 0, 9, 5), Ipv4Address(0, 0, 10, 5))},
        {Ipv4Address(192, 168, 107, 0), std::make_pair(Ipv4Address(0, 0, 9, 6), Ipv4Address(0, 0, 9, 1))},
        {Ipv4Address(192, 168, 108, 0), std::make_pair(Ipv4Address(0, 0, 9, 6), Ipv4Address(0, 0, 10, 6))},
        {Ipv4Address(192, 168, 109, 0), std::make_pair(Ipv4Address(0, 0, 10, 1), Ipv4Address(0, 0, 10, 2))},
        {Ipv4Address(192, 168, 110, 0), std::make_pair(Ipv4Address(0, 0, 10, 1), Ipv4Address(0, 0, 11, 1))},
        {Ipv4Address(192, 168, 111, 0), std::make_pair(Ipv4Address(0, 0, 10, 2), Ipv4Address(0, 0, 10, 3))},
        {Ipv4Address(192, 168, 112, 0), std::make_pair(Ipv4Address(0, 0, 10, 2), Ipv4Address(0, 0, 11, 2))},
        {Ipv4Address(192, 168, 113, 0), std::make_pair(Ipv4Address(0, 0, 10, 3), Ipv4Address(0, 0, 10, 4))},
        {Ipv4Address(192, 168, 114, 0), std::make_pair(Ipv4Address(0, 0, 10, 3), Ipv4Address(0, 0, 11, 3))},
        {Ipv4Address(192, 168, 115, 0), std::make_pair(Ipv4Address(0, 0, 10, 4), Ipv4Address(0, 0, 10, 5))},
        {Ipv4Address(192, 168, 116, 0), std::make_pair(Ipv4Address(0, 0, 10, 4), Ipv4Address(0, 0, 11, 4))},
        {Ipv4Address(192, 168, 117, 0), std::make_pair(Ipv4Address(0, 0, 10, 5), Ipv4Address(0, 0, 10, 6))},
        {Ipv4Address(192, 168, 118, 0), std::make_pair(Ipv4Address(0, 0, 10, 5), Ipv4Address(0, 0, 11, 5))},
        {Ipv4Address(192, 168, 119, 0), std::make_pair(Ipv4Address(0, 0, 10, 6), Ipv4Address(0, 0, 10, 1))},
        {Ipv4Address(192, 168, 120, 0), std::make_pair(Ipv4Address(0, 0, 10, 6), Ipv4Address(0, 0, 11, 6))},
        {Ipv4Address(192, 168, 121, 0), std::make_pair(Ipv4Address(0, 0, 11, 1), Ipv4Address(0, 0, 11, 2))},
        {Ipv4Address(192, 168, 122, 0), std::make_pair(Ipv4Address(0, 0, 11, 1), Ipv4Address(0, 0, 1, 1))},
        {Ipv4Address(192, 168, 123, 0), std::make_pair(Ipv4Address(0, 0, 11, 2), Ipv4Address(0, 0, 11, 3))},
        {Ipv4Address(192, 168, 124, 0), std::make_pair(Ipv4Address(0, 0, 11, 2), Ipv4Address(0, 0, 1, 2))},
        {Ipv4Address(192, 168, 125, 0), std::make_pair(Ipv4Address(0, 0, 11, 3), Ipv4Address(0, 0, 11, 4))},
        {Ipv4Address(192, 168, 126, 0), std::make_pair(Ipv4Address(0, 0, 11, 3), Ipv4Address(0, 0, 1, 3))},
        {Ipv4Address(192, 168, 127, 0), std::make_pair(Ipv4Address(0, 0, 11, 4), Ipv4Address(0, 0, 11, 5))},
        {Ipv4Address(192, 168, 128, 0), std::make_pair(Ipv4Address(0, 0, 11, 4), Ipv4Address(0, 0, 1, 4))},
        {Ipv4Address(192, 168, 129, 0), std::make_pair(Ipv4Address(0, 0, 11, 5), Ipv4Address(0, 0, 11, 6))},
        {Ipv4Address(192, 168, 130, 0), std::make_pair(Ipv4Address(0, 0, 11, 5), Ipv4Address(0, 0, 1, 5))},
        {Ipv4Address(192, 168, 131, 0), std::make_pair(Ipv4Address(0, 0, 11, 6), Ipv4Address(0, 0, 11, 1))},
        {Ipv4Address(192, 168, 132, 0), std::make_pair(Ipv4Address(0, 0, 11, 6), Ipv4Address(0, 0, 1, 6))}
};
const std::map<Ipv4Address, std::vector<Ipv4Address> > networksByRouterID = {
        {Ipv4Address(0, 0, 1, 1), {Ipv4Address(192, 168, 122, 1), Ipv4Address(192, 168, 2, 2), Ipv4Address(192, 168, 11, 1), Ipv4Address(192, 168, 1, 2)}},
        {Ipv4Address(0, 0, 1, 2), {Ipv4Address(192, 168, 124, 1), Ipv4Address(192, 168, 4, 2), Ipv4Address(192, 168, 1, 1), Ipv4Address(192, 168, 3, 2)}},
        {Ipv4Address(0, 0, 1, 3), {Ipv4Address(192, 168, 126, 1), Ipv4Address(192, 168, 6, 2), Ipv4Address(192, 168, 3, 1), Ipv4Address(192, 168, 5, 2)}},
        {Ipv4Address(0, 0, 1, 4), {Ipv4Address(192, 168, 128, 1), Ipv4Address(192, 168, 8, 2), Ipv4Address(192, 168, 5, 1), Ipv4Address(192, 168, 7, 2)}},
        {Ipv4Address(0, 0, 1, 5), {Ipv4Address(192, 168, 130, 1), Ipv4Address(192, 168, 10, 2), Ipv4Address(192, 168, 7, 1), Ipv4Address(192, 168, 9, 2)}},
        {Ipv4Address(0, 0, 1, 6), {Ipv4Address(192, 168, 132, 1), Ipv4Address(192, 168, 12, 2), Ipv4Address(192, 168, 9, 1), Ipv4Address(192, 168, 11, 2)}},
        {Ipv4Address(0, 0, 2, 1), {Ipv4Address(192, 168, 2, 1), Ipv4Address(192, 168, 14, 2), Ipv4Address(192, 168, 23, 1), Ipv4Address(192, 168, 13, 2)}},
        {Ipv4Address(0, 0, 2, 2), {Ipv4Address(192, 168, 4, 1), Ipv4Address(192, 168, 16, 2), Ipv4Address(192, 168, 13, 1), Ipv4Address(192, 168, 15, 2)}},
        {Ipv4Address(0, 0, 2, 3), {Ipv4Address(192, 168, 6, 1), Ipv4Address(192, 168, 18, 2), Ipv4Address(192, 168, 15, 1), Ipv4Address(192, 168, 17, 2)}},
        {Ipv4Address(0, 0, 2, 4), {Ipv4Address(192, 168, 8, 1), Ipv4Address(192, 168, 20, 2), Ipv4Address(192, 168, 17, 1), Ipv4Address(192, 168, 19, 2)}},
        {Ipv4Address(0, 0, 2, 5), {Ipv4Address(192, 168, 10, 1), Ipv4Address(192, 168, 22, 2), Ipv4Address(192, 168, 19, 1), Ipv4Address(192, 168, 21, 2)}},
        {Ipv4Address(0, 0, 2, 6), {Ipv4Address(192, 168, 12, 1), Ipv4Address(192, 168, 24, 2), Ipv4Address(192, 168, 21, 1), Ipv4Address(192, 168, 23, 2)}},
        {Ipv4Address(0, 0, 3, 1), {Ipv4Address(192, 168, 14, 1), Ipv4Address(192, 168, 26, 2), Ipv4Address(192, 168, 35, 1), Ipv4Address(192, 168, 25, 2)}},
        {Ipv4Address(0, 0, 3, 2), {Ipv4Address(192, 168, 16, 1), Ipv4Address(192, 168, 28, 2), Ipv4Address(192, 168, 25, 1), Ipv4Address(192, 168, 27, 2)}},
        {Ipv4Address(0, 0, 3, 3), {Ipv4Address(192, 168, 18, 1), Ipv4Address(192, 168, 30, 2), Ipv4Address(192, 168, 27, 1), Ipv4Address(192, 168, 29, 2)}},
        {Ipv4Address(0, 0, 3, 4), {Ipv4Address(192, 168, 20, 1), Ipv4Address(192, 168, 32, 2), Ipv4Address(192, 168, 29, 1), Ipv4Address(192, 168, 31, 2)}},
        {Ipv4Address(0, 0, 3, 5), {Ipv4Address(192, 168, 22, 1), Ipv4Address(192, 168, 34, 2), Ipv4Address(192, 168, 31, 1), Ipv4Address(192, 168, 33, 2)}},
        {Ipv4Address(0, 0, 3, 6), {Ipv4Address(192, 168, 24, 1), Ipv4Address(192, 168, 36, 2), Ipv4Address(192, 168, 33, 1), Ipv4Address(192, 168, 35, 2)}},
        {Ipv4Address(0, 0, 4, 1), {Ipv4Address(192, 168, 26, 1), Ipv4Address(192, 168, 38, 2), Ipv4Address(192, 168, 47, 1), Ipv4Address(192, 168, 37, 2)}},
        {Ipv4Address(0, 0, 4, 2), {Ipv4Address(192, 168, 28, 1), Ipv4Address(192, 168, 40, 2), Ipv4Address(192, 168, 37, 1), Ipv4Address(192, 168, 39, 2)}},
        {Ipv4Address(0, 0, 4, 3), {Ipv4Address(192, 168, 30, 1), Ipv4Address(192, 168, 42, 2), Ipv4Address(192, 168, 39, 1), Ipv4Address(192, 168, 41, 2)}},
        {Ipv4Address(0, 0, 4, 4), {Ipv4Address(192, 168, 32, 1), Ipv4Address(192, 168, 44, 2), Ipv4Address(192, 168, 41, 1), Ipv4Address(192, 168, 43, 2)}},
        {Ipv4Address(0, 0, 4, 5), {Ipv4Address(192, 168, 34, 1), Ipv4Address(192, 168, 46, 2), Ipv4Address(192, 168, 43, 1), Ipv4Address(192, 168, 45, 2)}},
        {Ipv4Address(0, 0, 4, 6), {Ipv4Address(192, 168, 36, 1), Ipv4Address(192, 168, 48, 2), Ipv4Address(192, 168, 45, 1), Ipv4Address(192, 168, 47, 2)}},
        {Ipv4Address(0, 0, 5, 1), {Ipv4Address(192, 168, 38, 1), Ipv4Address(192, 168, 50, 2), Ipv4Address(192, 168, 59, 1), Ipv4Address(192, 168, 49, 2)}},
        {Ipv4Address(0, 0, 5, 2), {Ipv4Address(192, 168, 40, 1), Ipv4Address(192, 168, 52, 2), Ipv4Address(192, 168, 49, 1), Ipv4Address(192, 168, 51, 2)}},
        {Ipv4Address(0, 0, 5, 3), {Ipv4Address(192, 168, 42, 1), Ipv4Address(192, 168, 54, 2), Ipv4Address(192, 168, 51, 1), Ipv4Address(192, 168, 53, 2)}},
        {Ipv4Address(0, 0, 5, 4), {Ipv4Address(192, 168, 44, 1), Ipv4Address(192, 168, 56, 2), Ipv4Address(192, 168, 53, 1), Ipv4Address(192, 168, 55, 2)}},
        {Ipv4Address(0, 0, 5, 5), {Ipv4Address(192, 168, 46, 1), Ipv4Address(192, 168, 58, 2), Ipv4Address(192, 168, 55, 1), Ipv4Address(192, 168, 57, 2)}},
        {Ipv4Address(0, 0, 5, 6), {Ipv4Address(192, 168, 48, 1), Ipv4Address(192, 168, 60, 2), Ipv4Address(192, 168, 57, 1), Ipv4Address(192, 168, 59, 2)}},
        {Ipv4Address(0, 0, 6, 1), {Ipv4Address(192, 168, 50, 1), Ipv4Address(192, 168, 62, 2), Ipv4Address(192, 168, 71, 1), Ipv4Address(192, 168, 61, 2)}},
        {Ipv4Address(0, 0, 6, 2), {Ipv4Address(192, 168, 52, 1), Ipv4Address(192, 168, 64, 2), Ipv4Address(192, 168, 61, 1), Ipv4Address(192, 168, 63, 2)}},
        {Ipv4Address(0, 0, 6, 3), {Ipv4Address(192, 168, 54, 1), Ipv4Address(192, 168, 66, 2), Ipv4Address(192, 168, 63, 1), Ipv4Address(192, 168, 65, 2)}},
        {Ipv4Address(0, 0, 6, 4), {Ipv4Address(192, 168, 56, 1), Ipv4Address(192, 168, 68, 2), Ipv4Address(192, 168, 65, 1), Ipv4Address(192, 168, 67, 2)}},
        {Ipv4Address(0, 0, 6, 5), {Ipv4Address(192, 168, 58, 1), Ipv4Address(192, 168, 70, 2), Ipv4Address(192, 168, 67, 1), Ipv4Address(192, 168, 69, 2)}},
        {Ipv4Address(0, 0, 6, 6), {Ipv4Address(192, 168, 60, 1), Ipv4Address(192, 168, 72, 2), Ipv4Address(192, 168, 69, 1), Ipv4Address(192, 168, 71, 2)}},
        {Ipv4Address(0, 0, 7, 1), {Ipv4Address(192, 168, 62, 1), Ipv4Address(192, 168, 74, 2), Ipv4Address(192, 168, 83, 1), Ipv4Address(192, 168, 73, 2)}},
        {Ipv4Address(0, 0, 7, 2), {Ipv4Address(192, 168, 64, 1), Ipv4Address(192, 168, 76, 2), Ipv4Address(192, 168, 73, 1), Ipv4Address(192, 168, 75, 2)}},
        {Ipv4Address(0, 0, 7, 3), {Ipv4Address(192, 168, 66, 1), Ipv4Address(192, 168, 78, 2), Ipv4Address(192, 168, 75, 1), Ipv4Address(192, 168, 77, 2)}},
        {Ipv4Address(0, 0, 7, 4), {Ipv4Address(192, 168, 68, 1), Ipv4Address(192, 168, 80, 2), Ipv4Address(192, 168, 77, 1), Ipv4Address(192, 168, 79, 2)}},
        {Ipv4Address(0, 0, 7, 5), {Ipv4Address(192, 168, 70, 1), Ipv4Address(192, 168, 82, 2), Ipv4Address(192, 168, 79, 1), Ipv4Address(192, 168, 81, 2)}},
        {Ipv4Address(0, 0, 7, 6), {Ipv4Address(192, 168, 72, 1), Ipv4Address(192, 168, 84, 2), Ipv4Address(192, 168, 81, 1), Ipv4Address(192, 168, 83, 2)}},
        {Ipv4Address(0, 0, 8, 1), {Ipv4Address(192, 168, 74, 1), Ipv4Address(192, 168, 86, 2), Ipv4Address(192, 168, 95, 1), Ipv4Address(192, 168, 85, 2)}},
        {Ipv4Address(0, 0, 8, 2), {Ipv4Address(192, 168, 76, 1), Ipv4Address(192, 168, 88, 2), Ipv4Address(192, 168, 85, 1), Ipv4Address(192, 168, 87, 2)}},
        {Ipv4Address(0, 0, 8, 3), {Ipv4Address(192, 168, 78, 1), Ipv4Address(192, 168, 90, 2), Ipv4Address(192, 168, 87, 1), Ipv4Address(192, 168, 89, 2)}},
        {Ipv4Address(0, 0, 8, 4), {Ipv4Address(192, 168, 80, 1), Ipv4Address(192, 168, 92, 2), Ipv4Address(192, 168, 89, 1), Ipv4Address(192, 168, 91, 2)}},
        {Ipv4Address(0, 0, 8, 5), {Ipv4Address(192, 168, 82, 1), Ipv4Address(192, 168, 94, 2), Ipv4Address(192, 168, 91, 1), Ipv4Address(192, 168, 93, 2)}},
        {Ipv4Address(0, 0, 8, 6), {Ipv4Address(192, 168, 84, 1), Ipv4Address(192, 168, 96, 2), Ipv4Address(192, 168, 93, 1), Ipv4Address(192, 168, 95, 2)}},
        {Ipv4Address(0, 0, 9, 1), {Ipv4Address(192, 168, 86, 1), Ipv4Address(192, 168, 98, 2), Ipv4Address(192, 168, 107, 1), Ipv4Address(192, 168, 97, 2)}},
        {Ipv4Address(0, 0, 9, 2), {Ipv4Address(192, 168, 88, 1), Ipv4Address(192, 168, 100, 2), Ipv4Address(192, 168, 97, 1), Ipv4Address(192, 168, 99, 2)}},
        {Ipv4Address(0, 0, 9, 3), {Ipv4Address(192, 168, 90, 1), Ipv4Address(192, 168, 102, 2), Ipv4Address(192, 168, 99, 1), Ipv4Address(192, 168, 101, 2)}},
        {Ipv4Address(0, 0, 9, 4), {Ipv4Address(192, 168, 92, 1), Ipv4Address(192, 168, 104, 2), Ipv4Address(192, 168, 101, 1), Ipv4Address(192, 168, 103, 2)}},
        {Ipv4Address(0, 0, 9, 5), {Ipv4Address(192, 168, 94, 1), Ipv4Address(192, 168, 106, 2), Ipv4Address(192, 168, 103, 1), Ipv4Address(192, 168, 105, 2)}},
        {Ipv4Address(0, 0, 9, 6), {Ipv4Address(192, 168, 96, 1), Ipv4Address(192, 168, 108, 2), Ipv4Address(192, 168, 105, 1), Ipv4Address(192, 168, 107, 2)}},
        {Ipv4Address(0, 0, 10, 1), {Ipv4Address(192, 168, 98, 1), Ipv4Address(192, 168, 110, 2), Ipv4Address(192, 168, 119, 1), Ipv4Address(192, 168, 109, 2)}},
        {Ipv4Address(0, 0, 10, 2), {Ipv4Address(192, 168, 100, 1), Ipv4Address(192, 168, 112, 2), Ipv4Address(192, 168, 109, 1), Ipv4Address(192, 168, 111, 2)}},
        {Ipv4Address(0, 0, 10, 3), {Ipv4Address(192, 168, 102, 1), Ipv4Address(192, 168, 114, 2), Ipv4Address(192, 168, 111, 1), Ipv4Address(192, 168, 113, 2)}},
        {Ipv4Address(0, 0, 10, 4), {Ipv4Address(192, 168, 104, 1), Ipv4Address(192, 168, 116, 2), Ipv4Address(192, 168, 113, 1), Ipv4Address(192, 168, 115, 2)}},
        {Ipv4Address(0, 0, 10, 5), {Ipv4Address(192, 168, 106, 1), Ipv4Address(192, 168, 118, 2), Ipv4Address(192, 168, 115, 1), Ipv4Address(192, 168, 117, 2)}},
        {Ipv4Address(0, 0, 10, 6), {Ipv4Address(192, 168, 108, 1), Ipv4Address(192, 168, 120, 2), Ipv4Address(192, 168, 117, 1), Ipv4Address(192, 168, 119, 2)}},
        {Ipv4Address(0, 0, 11, 1), {Ipv4Address(192, 168, 110, 1), Ipv4Address(192, 168, 122, 2), Ipv4Address(192, 168, 131, 1), Ipv4Address(192, 168, 121, 2)}},
        {Ipv4Address(0, 0, 11, 2), {Ipv4Address(192, 168, 112, 1), Ipv4Address(192, 168, 124, 2), Ipv4Address(192, 168, 121, 1), Ipv4Address(192, 168, 123, 2)}},
        {Ipv4Address(0, 0, 11, 3), {Ipv4Address(192, 168, 114, 1), Ipv4Address(192, 168, 126, 2), Ipv4Address(192, 168, 123, 1), Ipv4Address(192, 168, 125, 2)}},
        {Ipv4Address(0, 0, 11, 4), {Ipv4Address(192, 168, 116, 1), Ipv4Address(192, 168, 128, 2), Ipv4Address(192, 168, 125, 1), Ipv4Address(192, 168, 127, 2)}},
        {Ipv4Address(0, 0, 11, 5), {Ipv4Address(192, 168, 118, 1), Ipv4Address(192, 168, 130, 2), Ipv4Address(192, 168, 127, 1), Ipv4Address(192, 168, 129, 2)}},
        {Ipv4Address(0, 0, 11, 6), {Ipv4Address(192, 168, 120, 1), Ipv4Address(192, 168, 132, 2), Ipv4Address(192, 168, 129, 1), Ipv4Address(192, 168, 131, 2)}}
};
const std::map <Ipv4Address, Ipv4Address> routerIDByIPAddress = {
        {Ipv4Address(192, 168, 122, 2), Ipv4Address(0, 0, 1, 1)},
        {Ipv4Address(192, 168, 2, 1), Ipv4Address(0, 0, 1, 1)},
        {Ipv4Address(192, 168, 11, 2), Ipv4Address(0, 0, 1, 1)},
        {Ipv4Address(192, 168, 1, 1), Ipv4Address(0, 0, 1, 1)},
        {Ipv4Address(192, 168, 124, 2), Ipv4Address(0, 0, 1, 2)},
        {Ipv4Address(192, 168, 4, 1), Ipv4Address(0, 0, 1, 2)},
        {Ipv4Address(192, 168, 1, 2), Ipv4Address(0, 0, 1, 2)},
        {Ipv4Address(192, 168, 3, 1), Ipv4Address(0, 0, 1, 2)},
        {Ipv4Address(192, 168, 126, 2), Ipv4Address(0, 0, 1, 3)},
        {Ipv4Address(192, 168, 6, 1), Ipv4Address(0, 0, 1, 3)},
        {Ipv4Address(192, 168, 3, 2), Ipv4Address(0, 0, 1, 3)},
        {Ipv4Address(192, 168, 5, 1), Ipv4Address(0, 0, 1, 3)},
        {Ipv4Address(192, 168, 128, 2), Ipv4Address(0, 0, 1, 4)},
        {Ipv4Address(192, 168, 8, 1), Ipv4Address(0, 0, 1, 4)},
        {Ipv4Address(192, 168, 5, 2), Ipv4Address(0, 0, 1, 4)},
        {Ipv4Address(192, 168, 7, 1), Ipv4Address(0, 0, 1, 4)},
        {Ipv4Address(192, 168, 130, 2), Ipv4Address(0, 0, 1, 5)},
        {Ipv4Address(192, 168, 10, 1), Ipv4Address(0, 0, 1, 5)},
        {Ipv4Address(192, 168, 7, 2), Ipv4Address(0, 0, 1, 5)},
        {Ipv4Address(192, 168, 9, 1), Ipv4Address(0, 0, 1, 5)},
        {Ipv4Address(192, 168, 132, 2), Ipv4Address(0, 0, 1, 6)},
        {Ipv4Address(192, 168, 12, 1), Ipv4Address(0, 0, 1, 6)},
        {Ipv4Address(192, 168, 9, 2), Ipv4Address(0, 0, 1, 6)},
        {Ipv4Address(192, 168, 11, 1), Ipv4Address(0, 0, 1, 6)},
        {Ipv4Address(192, 168, 2, 2), Ipv4Address(0, 0, 2, 1)},
        {Ipv4Address(192, 168, 14, 1), Ipv4Address(0, 0, 2, 1)},
        {Ipv4Address(192, 168, 23, 2), Ipv4Address(0, 0, 2, 1)},
        {Ipv4Address(192, 168, 13, 1), Ipv4Address(0, 0, 2, 1)},
        {Ipv4Address(192, 168, 4, 2), Ipv4Address(0, 0, 2, 2)},
        {Ipv4Address(192, 168, 16, 1), Ipv4Address(0, 0, 2, 2)},
        {Ipv4Address(192, 168, 13, 2), Ipv4Address(0, 0, 2, 2)},
        {Ipv4Address(192, 168, 15, 1), Ipv4Address(0, 0, 2, 2)},
        {Ipv4Address(192, 168, 6, 2), Ipv4Address(0, 0, 2, 3)},
        {Ipv4Address(192, 168, 18, 1), Ipv4Address(0, 0, 2, 3)},
        {Ipv4Address(192, 168, 15, 2), Ipv4Address(0, 0, 2, 3)},
        {Ipv4Address(192, 168, 17, 1), Ipv4Address(0, 0, 2, 3)},
        {Ipv4Address(192, 168, 8, 2), Ipv4Address(0, 0, 2, 4)},
        {Ipv4Address(192, 168, 20, 1), Ipv4Address(0, 0, 2, 4)},
        {Ipv4Address(192, 168, 17, 2), Ipv4Address(0, 0, 2, 4)},
        {Ipv4Address(192, 168, 19, 1), Ipv4Address(0, 0, 2, 4)},
        {Ipv4Address(192, 168, 10, 2), Ipv4Address(0, 0, 2, 5)},
        {Ipv4Address(192, 168, 22, 1), Ipv4Address(0, 0, 2, 5)},
        {Ipv4Address(192, 168, 19, 2), Ipv4Address(0, 0, 2, 5)},
        {Ipv4Address(192, 168, 21, 1), Ipv4Address(0, 0, 2, 5)},
        {Ipv4Address(192, 168, 12, 2), Ipv4Address(0, 0, 2, 6)},
        {Ipv4Address(192, 168, 24, 1), Ipv4Address(0, 0, 2, 6)},
        {Ipv4Address(192, 168, 21, 2), Ipv4Address(0, 0, 2, 6)},
        {Ipv4Address(192, 168, 23, 1), Ipv4Address(0, 0, 2, 6)},
        {Ipv4Address(192, 168, 14, 2), Ipv4Address(0, 0, 3, 1)},
        {Ipv4Address(192, 168, 26, 1), Ipv4Address(0, 0, 3, 1)},
        {Ipv4Address(192, 168, 35, 2), Ipv4Address(0, 0, 3, 1)},
        {Ipv4Address(192, 168, 25, 1), Ipv4Address(0, 0, 3, 1)},
        {Ipv4Address(192, 168, 16, 2), Ipv4Address(0, 0, 3, 2)},
        {Ipv4Address(192, 168, 28, 1), Ipv4Address(0, 0, 3, 2)},
        {Ipv4Address(192, 168, 25, 2), Ipv4Address(0, 0, 3, 2)},
        {Ipv4Address(192, 168, 27, 1), Ipv4Address(0, 0, 3, 2)},
        {Ipv4Address(192, 168, 18, 2), Ipv4Address(0, 0, 3, 3)},
        {Ipv4Address(192, 168, 30, 1), Ipv4Address(0, 0, 3, 3)},
        {Ipv4Address(192, 168, 27, 2), Ipv4Address(0, 0, 3, 3)},
        {Ipv4Address(192, 168, 29, 1), Ipv4Address(0, 0, 3, 3)},
        {Ipv4Address(192, 168, 20, 2), Ipv4Address(0, 0, 3, 4)},
        {Ipv4Address(192, 168, 32, 1), Ipv4Address(0, 0, 3, 4)},
        {Ipv4Address(192, 168, 29, 2), Ipv4Address(0, 0, 3, 4)},
        {Ipv4Address(192, 168, 31, 1), Ipv4Address(0, 0, 3, 4)},
        {Ipv4Address(192, 168, 22, 2), Ipv4Address(0, 0, 3, 5)},
        {Ipv4Address(192, 168, 34, 1), Ipv4Address(0, 0, 3, 5)},
        {Ipv4Address(192, 168, 31, 2), Ipv4Address(0, 0, 3, 5)},
        {Ipv4Address(192, 168, 33, 1), Ipv4Address(0, 0, 3, 5)},
        {Ipv4Address(192, 168, 24, 2), Ipv4Address(0, 0, 3, 6)},
        {Ipv4Address(192, 168, 36, 1), Ipv4Address(0, 0, 3, 6)},
        {Ipv4Address(192, 168, 33, 2), Ipv4Address(0, 0, 3, 6)},
        {Ipv4Address(192, 168, 35, 1), Ipv4Address(0, 0, 3, 6)},
        {Ipv4Address(192, 168, 26, 2), Ipv4Address(0, 0, 4, 1)},
        {Ipv4Address(192, 168, 38, 1), Ipv4Address(0, 0, 4, 1)},
        {Ipv4Address(192, 168, 47, 2), Ipv4Address(0, 0, 4, 1)},
        {Ipv4Address(192, 168, 37, 1), Ipv4Address(0, 0, 4, 1)},
        {Ipv4Address(192, 168, 28, 2), Ipv4Address(0, 0, 4, 2)},
        {Ipv4Address(192, 168, 40, 1), Ipv4Address(0, 0, 4, 2)},
        {Ipv4Address(192, 168, 37, 2), Ipv4Address(0, 0, 4, 2)},
        {Ipv4Address(192, 168, 39, 1), Ipv4Address(0, 0, 4, 2)},
        {Ipv4Address(192, 168, 30, 2), Ipv4Address(0, 0, 4, 3)},
        {Ipv4Address(192, 168, 42, 1), Ipv4Address(0, 0, 4, 3)},
        {Ipv4Address(192, 168, 39, 2), Ipv4Address(0, 0, 4, 3)},
        {Ipv4Address(192, 168, 41, 1), Ipv4Address(0, 0, 4, 3)},
        {Ipv4Address(192, 168, 32, 2), Ipv4Address(0, 0, 4, 4)},
        {Ipv4Address(192, 168, 44, 1), Ipv4Address(0, 0, 4, 4)},
        {Ipv4Address(192, 168, 41, 2), Ipv4Address(0, 0, 4, 4)},
        {Ipv4Address(192, 168, 43, 1), Ipv4Address(0, 0, 4, 4)},
        {Ipv4Address(192, 168, 34, 2), Ipv4Address(0, 0, 4, 5)},
        {Ipv4Address(192, 168, 46, 1), Ipv4Address(0, 0, 4, 5)},
        {Ipv4Address(192, 168, 43, 2), Ipv4Address(0, 0, 4, 5)},
        {Ipv4Address(192, 168, 45, 1), Ipv4Address(0, 0, 4, 5)},
        {Ipv4Address(192, 168, 36, 2), Ipv4Address(0, 0, 4, 6)},
        {Ipv4Address(192, 168, 48, 1), Ipv4Address(0, 0, 4, 6)},
        {Ipv4Address(192, 168, 45, 2), Ipv4Address(0, 0, 4, 6)},
        {Ipv4Address(192, 168, 47, 1), Ipv4Address(0, 0, 4, 6)},
        {Ipv4Address(192, 168, 38, 2), Ipv4Address(0, 0, 5, 1)},
        {Ipv4Address(192, 168, 50, 1), Ipv4Address(0, 0, 5, 1)},
        {Ipv4Address(192, 168, 59, 2), Ipv4Address(0, 0, 5, 1)},
        {Ipv4Address(192, 168, 49, 1), Ipv4Address(0, 0, 5, 1)},
        {Ipv4Address(192, 168, 40, 2), Ipv4Address(0, 0, 5, 2)},
        {Ipv4Address(192, 168, 52, 1), Ipv4Address(0, 0, 5, 2)},
        {Ipv4Address(192, 168, 49, 2), Ipv4Address(0, 0, 5, 2)},
        {Ipv4Address(192, 168, 51, 1), Ipv4Address(0, 0, 5, 2)},
        {Ipv4Address(192, 168, 42, 2), Ipv4Address(0, 0, 5, 3)},
        {Ipv4Address(192, 168, 54, 1), Ipv4Address(0, 0, 5, 3)},
        {Ipv4Address(192, 168, 51, 2), Ipv4Address(0, 0, 5, 3)},
        {Ipv4Address(192, 168, 53, 1), Ipv4Address(0, 0, 5, 3)},
        {Ipv4Address(192, 168, 44, 2), Ipv4Address(0, 0, 5, 4)},
        {Ipv4Address(192, 168, 56, 1), Ipv4Address(0, 0, 5, 4)},
        {Ipv4Address(192, 168, 53, 2), Ipv4Address(0, 0, 5, 4)},
        {Ipv4Address(192, 168, 55, 1), Ipv4Address(0, 0, 5, 4)},
        {Ipv4Address(192, 168, 46, 2), Ipv4Address(0, 0, 5, 5)},
        {Ipv4Address(192, 168, 58, 1), Ipv4Address(0, 0, 5, 5)},
        {Ipv4Address(192, 168, 55, 2), Ipv4Address(0, 0, 5, 5)},
        {Ipv4Address(192, 168, 57, 1), Ipv4Address(0, 0, 5, 5)},
        {Ipv4Address(192, 168, 48, 2), Ipv4Address(0, 0, 5, 6)},
        {Ipv4Address(192, 168, 60, 1), Ipv4Address(0, 0, 5, 6)},
        {Ipv4Address(192, 168, 57, 2), Ipv4Address(0, 0, 5, 6)},
        {Ipv4Address(192, 168, 59, 1), Ipv4Address(0, 0, 5, 6)},
        {Ipv4Address(192, 168, 50, 2), Ipv4Address(0, 0, 6, 1)},
        {Ipv4Address(192, 168, 62, 1), Ipv4Address(0, 0, 6, 1)},
        {Ipv4Address(192, 168, 71, 2), Ipv4Address(0, 0, 6, 1)},
        {Ipv4Address(192, 168, 61, 1), Ipv4Address(0, 0, 6, 1)},
        {Ipv4Address(192, 168, 52, 2), Ipv4Address(0, 0, 6, 2)},
        {Ipv4Address(192, 168, 64, 1), Ipv4Address(0, 0, 6, 2)},
        {Ipv4Address(192, 168, 61, 2), Ipv4Address(0, 0, 6, 2)},
        {Ipv4Address(192, 168, 63, 1), Ipv4Address(0, 0, 6, 2)},
        {Ipv4Address(192, 168, 54, 2), Ipv4Address(0, 0, 6, 3)},
        {Ipv4Address(192, 168, 66, 1), Ipv4Address(0, 0, 6, 3)},
        {Ipv4Address(192, 168, 63, 2), Ipv4Address(0, 0, 6, 3)},
        {Ipv4Address(192, 168, 65, 1), Ipv4Address(0, 0, 6, 3)},
        {Ipv4Address(192, 168, 56, 2), Ipv4Address(0, 0, 6, 4)},
        {Ipv4Address(192, 168, 68, 1), Ipv4Address(0, 0, 6, 4)},
        {Ipv4Address(192, 168, 65, 2), Ipv4Address(0, 0, 6, 4)},
        {Ipv4Address(192, 168, 67, 1), Ipv4Address(0, 0, 6, 4)},
        {Ipv4Address(192, 168, 58, 2), Ipv4Address(0, 0, 6, 5)},
        {Ipv4Address(192, 168, 70, 1), Ipv4Address(0, 0, 6, 5)},
        {Ipv4Address(192, 168, 67, 2), Ipv4Address(0, 0, 6, 5)},
        {Ipv4Address(192, 168, 69, 1), Ipv4Address(0, 0, 6, 5)},
        {Ipv4Address(192, 168, 60, 2), Ipv4Address(0, 0, 6, 6)},
        {Ipv4Address(192, 168, 72, 1), Ipv4Address(0, 0, 6, 6)},
        {Ipv4Address(192, 168, 69, 2), Ipv4Address(0, 0, 6, 6)},
        {Ipv4Address(192, 168, 71, 1), Ipv4Address(0, 0, 6, 6)},
        {Ipv4Address(192, 168, 62, 2), Ipv4Address(0, 0, 7, 1)},
        {Ipv4Address(192, 168, 74, 1), Ipv4Address(0, 0, 7, 1)},
        {Ipv4Address(192, 168, 83, 2), Ipv4Address(0, 0, 7, 1)},
        {Ipv4Address(192, 168, 73, 1), Ipv4Address(0, 0, 7, 1)},
        {Ipv4Address(192, 168, 64, 2), Ipv4Address(0, 0, 7, 2)},
        {Ipv4Address(192, 168, 76, 1), Ipv4Address(0, 0, 7, 2)},
        {Ipv4Address(192, 168, 73, 2), Ipv4Address(0, 0, 7, 2)},
        {Ipv4Address(192, 168, 75, 1), Ipv4Address(0, 0, 7, 2)},
        {Ipv4Address(192, 168, 66, 2), Ipv4Address(0, 0, 7, 3)},
        {Ipv4Address(192, 168, 78, 1), Ipv4Address(0, 0, 7, 3)},
        {Ipv4Address(192, 168, 75, 2), Ipv4Address(0, 0, 7, 3)},
        {Ipv4Address(192, 168, 77, 1), Ipv4Address(0, 0, 7, 3)},
        {Ipv4Address(192, 168, 68, 2), Ipv4Address(0, 0, 7, 4)},
        {Ipv4Address(192, 168, 80, 1), Ipv4Address(0, 0, 7, 4)},
        {Ipv4Address(192, 168, 77, 2), Ipv4Address(0, 0, 7, 4)},
        {Ipv4Address(192, 168, 79, 1), Ipv4Address(0, 0, 7, 4)},
        {Ipv4Address(192, 168, 70, 2), Ipv4Address(0, 0, 7, 5)},
        {Ipv4Address(192, 168, 82, 1), Ipv4Address(0, 0, 7, 5)},
        {Ipv4Address(192, 168, 79, 2), Ipv4Address(0, 0, 7, 5)},
        {Ipv4Address(192, 168, 81, 1), Ipv4Address(0, 0, 7, 5)},
        {Ipv4Address(192, 168, 72, 2), Ipv4Address(0, 0, 7, 6)},
        {Ipv4Address(192, 168, 84, 1), Ipv4Address(0, 0, 7, 6)},
        {Ipv4Address(192, 168, 81, 2), Ipv4Address(0, 0, 7, 6)},
        {Ipv4Address(192, 168, 83, 1), Ipv4Address(0, 0, 7, 6)},
        {Ipv4Address(192, 168, 74, 2), Ipv4Address(0, 0, 8, 1)},
        {Ipv4Address(192, 168, 86, 1), Ipv4Address(0, 0, 8, 1)},
        {Ipv4Address(192, 168, 95, 2), Ipv4Address(0, 0, 8, 1)},
        {Ipv4Address(192, 168, 85, 1), Ipv4Address(0, 0, 8, 1)},
        {Ipv4Address(192, 168, 76, 2), Ipv4Address(0, 0, 8, 2)},
        {Ipv4Address(192, 168, 88, 1), Ipv4Address(0, 0, 8, 2)},
        {Ipv4Address(192, 168, 85, 2), Ipv4Address(0, 0, 8, 2)},
        {Ipv4Address(192, 168, 87, 1), Ipv4Address(0, 0, 8, 2)},
        {Ipv4Address(192, 168, 78, 2), Ipv4Address(0, 0, 8, 3)},
        {Ipv4Address(192, 168, 90, 1), Ipv4Address(0, 0, 8, 3)},
        {Ipv4Address(192, 168, 87, 2), Ipv4Address(0, 0, 8, 3)},
        {Ipv4Address(192, 168, 89, 1), Ipv4Address(0, 0, 8, 3)},
        {Ipv4Address(192, 168, 80, 2), Ipv4Address(0, 0, 8, 4)},
        {Ipv4Address(192, 168, 92, 1), Ipv4Address(0, 0, 8, 4)},
        {Ipv4Address(192, 168, 89, 2), Ipv4Address(0, 0, 8, 4)},
        {Ipv4Address(192, 168, 91, 1), Ipv4Address(0, 0, 8, 4)},
        {Ipv4Address(192, 168, 82, 2), Ipv4Address(0, 0, 8, 5)},
        {Ipv4Address(192, 168, 94, 1), Ipv4Address(0, 0, 8, 5)},
        {Ipv4Address(192, 168, 91, 2), Ipv4Address(0, 0, 8, 5)},
        {Ipv4Address(192, 168, 93, 1), Ipv4Address(0, 0, 8, 5)},
        {Ipv4Address(192, 168, 84, 2), Ipv4Address(0, 0, 8, 6)},
        {Ipv4Address(192, 168, 96, 1), Ipv4Address(0, 0, 8, 6)},
        {Ipv4Address(192, 168, 93, 2), Ipv4Address(0, 0, 8, 6)},
        {Ipv4Address(192, 168, 95, 1), Ipv4Address(0, 0, 8, 6)},
        {Ipv4Address(192, 168, 86, 2), Ipv4Address(0, 0, 9, 1)},
        {Ipv4Address(192, 168, 98, 1), Ipv4Address(0, 0, 9, 1)},
        {Ipv4Address(192, 168, 107, 2), Ipv4Address(0, 0, 9, 1)},
        {Ipv4Address(192, 168, 97, 1), Ipv4Address(0, 0, 9, 1)},
        {Ipv4Address(192, 168, 88, 2), Ipv4Address(0, 0, 9, 2)},
        {Ipv4Address(192, 168, 100, 1), Ipv4Address(0, 0, 9, 2)},
        {Ipv4Address(192, 168, 97, 2), Ipv4Address(0, 0, 9, 2)},
        {Ipv4Address(192, 168, 99, 1), Ipv4Address(0, 0, 9, 2)},
        {Ipv4Address(192, 168, 90, 2), Ipv4Address(0, 0, 9, 3)},
        {Ipv4Address(192, 168, 102, 1), Ipv4Address(0, 0, 9, 3)},
        {Ipv4Address(192, 168, 99, 2), Ipv4Address(0, 0, 9, 3)},
        {Ipv4Address(192, 168, 101, 1), Ipv4Address(0, 0, 9, 3)},
        {Ipv4Address(192, 168, 92, 2), Ipv4Address(0, 0, 9, 4)},
        {Ipv4Address(192, 168, 104, 1), Ipv4Address(0, 0, 9, 4)},
        {Ipv4Address(192, 168, 101, 2), Ipv4Address(0, 0, 9, 4)},
        {Ipv4Address(192, 168, 103, 1), Ipv4Address(0, 0, 9, 4)},
        {Ipv4Address(192, 168, 94, 2), Ipv4Address(0, 0, 9, 5)},
        {Ipv4Address(192, 168, 106, 1), Ipv4Address(0, 0, 9, 5)},
        {Ipv4Address(192, 168, 103, 2), Ipv4Address(0, 0, 9, 5)},
        {Ipv4Address(192, 168, 105, 1), Ipv4Address(0, 0, 9, 5)},
        {Ipv4Address(192, 168, 96, 2), Ipv4Address(0, 0, 9, 6)},
        {Ipv4Address(192, 168, 108, 1), Ipv4Address(0, 0, 9, 6)},
        {Ipv4Address(192, 168, 105, 2), Ipv4Address(0, 0, 9, 6)},
        {Ipv4Address(192, 168, 107, 1), Ipv4Address(0, 0, 9, 6)},
        {Ipv4Address(192, 168, 98, 2), Ipv4Address(0, 0, 10, 1)},
        {Ipv4Address(192, 168, 110, 1), Ipv4Address(0, 0, 10, 1)},
        {Ipv4Address(192, 168, 119, 2), Ipv4Address(0, 0, 10, 1)},
        {Ipv4Address(192, 168, 109, 1), Ipv4Address(0, 0, 10, 1)},
        {Ipv4Address(192, 168, 100, 2), Ipv4Address(0, 0, 10, 2)},
        {Ipv4Address(192, 168, 112, 1), Ipv4Address(0, 0, 10, 2)},
        {Ipv4Address(192, 168, 109, 2), Ipv4Address(0, 0, 10, 2)},
        {Ipv4Address(192, 168, 111, 1), Ipv4Address(0, 0, 10, 2)},
        {Ipv4Address(192, 168, 102, 2), Ipv4Address(0, 0, 10, 3)},
        {Ipv4Address(192, 168, 114, 1), Ipv4Address(0, 0, 10, 3)},
        {Ipv4Address(192, 168, 111, 2), Ipv4Address(0, 0, 10, 3)},
        {Ipv4Address(192, 168, 113, 1), Ipv4Address(0, 0, 10, 3)},
        {Ipv4Address(192, 168, 104, 2), Ipv4Address(0, 0, 10, 4)},
        {Ipv4Address(192, 168, 116, 1), Ipv4Address(0, 0, 10, 4)},
        {Ipv4Address(192, 168, 113, 2), Ipv4Address(0, 0, 10, 4)},
        {Ipv4Address(192, 168, 115, 1), Ipv4Address(0, 0, 10, 4)},
        {Ipv4Address(192, 168, 106, 2), Ipv4Address(0, 0, 10, 5)},
        {Ipv4Address(192, 168, 118, 1), Ipv4Address(0, 0, 10, 5)},
        {Ipv4Address(192, 168, 115, 2), Ipv4Address(0, 0, 10, 5)},
        {Ipv4Address(192, 168, 117, 1), Ipv4Address(0, 0, 10, 5)},
        {Ipv4Address(192, 168, 108, 2), Ipv4Address(0, 0, 10, 6)},
        {Ipv4Address(192, 168, 120, 1), Ipv4Address(0, 0, 10, 6)},
        {Ipv4Address(192, 168, 117, 2), Ipv4Address(0, 0, 10, 6)},
        {Ipv4Address(192, 168, 119, 1), Ipv4Address(0, 0, 10, 6)},
        {Ipv4Address(192, 168, 110, 2), Ipv4Address(0, 0, 11, 1)},
        {Ipv4Address(192, 168, 122, 1), Ipv4Address(0, 0, 11, 1)},
        {Ipv4Address(192, 168, 131, 2), Ipv4Address(0, 0, 11, 1)},
        {Ipv4Address(192, 168, 121, 1), Ipv4Address(0, 0, 11, 1)},
        {Ipv4Address(192, 168, 112, 2), Ipv4Address(0, 0, 11, 2)},
        {Ipv4Address(192, 168, 124, 1), Ipv4Address(0, 0, 11, 2)},
        {Ipv4Address(192, 168, 121, 2), Ipv4Address(0, 0, 11, 2)},
        {Ipv4Address(192, 168, 123, 1), Ipv4Address(0, 0, 11, 2)},
        {Ipv4Address(192, 168, 114, 2), Ipv4Address(0, 0, 11, 3)},
        {Ipv4Address(192, 168, 126, 1), Ipv4Address(0, 0, 11, 3)},
        {Ipv4Address(192, 168, 123, 2), Ipv4Address(0, 0, 11, 3)},
        {Ipv4Address(192, 168, 125, 1), Ipv4Address(0, 0, 11, 3)},
        {Ipv4Address(192, 168, 116, 2), Ipv4Address(0, 0, 11, 4)},
        {Ipv4Address(192, 168, 128, 1), Ipv4Address(0, 0, 11, 4)},
        {Ipv4Address(192, 168, 125, 2), Ipv4Address(0, 0, 11, 4)},
        {Ipv4Address(192, 168, 127, 1), Ipv4Address(0, 0, 11, 4)},
        {Ipv4Address(192, 168, 118, 2), Ipv4Address(0, 0, 11, 5)},
        {Ipv4Address(192, 168, 130, 1), Ipv4Address(0, 0, 11, 5)},
        {Ipv4Address(192, 168, 127, 2), Ipv4Address(0, 0, 11, 5)},
        {Ipv4Address(192, 168, 129, 1), Ipv4Address(0, 0, 11, 5)},
        {Ipv4Address(192, 168, 120, 2), Ipv4Address(0, 0, 11, 6)},
        {Ipv4Address(192, 168, 132, 1), Ipv4Address(0, 0, 11, 6)},
        {Ipv4Address(192, 168, 129, 2), Ipv4Address(0, 0, 11, 6)},
        {Ipv4Address(192, 168, 131, 1), Ipv4Address(0, 0, 11, 6)},
};


/*
 * @sqsq
 * if it is sure that at this time, the network is converged, return true
 */
inline bool sqsqCheckSimTime()
{
    return (simTime() > SimTime(SQSQ_CONVERGENCY_TIME));
//    std::cout << "warm: " << getSimulation()->getWarmupPeriod() << std::endl;
//    return simTime() > getSimulation()->getWarmupPeriod();
}

inline int sqsqRescaleM(int num)
{
    if (num > SQSQ_M) return num % SQSQ_M;
    else if (num <= 0) return num + SQSQ_M;
    else return num;
}

inline int sqsqRescaleN(int num)
{
    if (num > SQSQ_N) return num % SQSQ_N;
    else if (num <= 0) return num + SQSQ_N;
    else return num;
}

inline int sqsqCalculateManhattanDistance(Ipv4Address addr1, Ipv4Address addr2)
{
    return abs(addr1.getDByte(2) - addr2.getDByte(2)) + abs(addr1.getDByte(3) - addr2.getDByte(3));
}

const B IPV4_DATAGRAM_LENGTH                   = B(65536);
const B OSPFv2_HEADER_LENGTH                   = B(24);
const B OSPFv2_LSA_HEADER_LENGTH               = B(20);
const B OSPFv2_HELLO_HEADER_LENGTH             = B(20);
const B OSPFv2_DD_HEADER_LENGTH                = B(8);
const B OSPFv2_REQUEST_LENGTH                  = B(12);
const B OSPFv2_ROUTERLSA_HEADER_LENGTH         = B(4);
const B OSPFv2_LINK_HEADER_LENGTH              = B(12);
const B OSPFv2_TOS_LENGTH                      = B(4);
const B OSPFv2_NETWORKLSA_MASK_LENGTH          = B(4);
const B OSPFv2_NETWORKLSA_ADDRESS_LENGTH       = B(4);
const B OSPFv2_SUMMARYLSA_HEADER_LENGTH        = B(8);
const B OSPFv2_ASEXTERNALLSA_HEADER_LENGTH     = B(4);
const B OSPFv2_ASEXTERNALLSA_TOS_INFO_LENGTH   = B(12);

#define OSPFv2_EXTERNAL_ROUTES_LEARNED_BY_BGP  179
#define OSPFv2_BGP_DEFAULT_COST                1

typedef unsigned long Metric;

enum AuthenticationType {
    NULL_TYPE            = 0,
    SIMPLE_PASSWORD_TYPE = 1,
    CRYTOGRAPHIC_TYPE    = 2
};

struct AuthenticationKeyType
{
    char bytes[8];
};

struct Ipv4AddressRange
{
    Ipv4Address address;
    Ipv4Address mask;
    Ipv4AddressRange() : address(), mask() {}
    Ipv4AddressRange(Ipv4Address addressPar, Ipv4Address maskPar) : address(addressPar), mask(maskPar) {}

    bool operator<(const Ipv4AddressRange& other) const
    {
        return (mask > other.mask) || ((mask == other.mask) && (address < other.address));
    }

    bool operator==(const Ipv4AddressRange& other) const
    {
        return (address == other.address) && (mask == other.mask);
    }

    bool contains(const Ipv4Address& other) const
    {
        return Ipv4Address::maskedAddrAreEqual(address, other, mask);
    }

    bool contains(const Ipv4AddressRange& other) const
    {
        return Ipv4Address::maskedAddrAreEqual(address, other.address, mask) && (mask <= other.mask);
    }

    bool containsRange(const Ipv4Address& otherAddress, const Ipv4Address& otherMask) const
    {
        return Ipv4Address::maskedAddrAreEqual(address, otherAddress, mask) && (mask <= otherMask);
    }

    bool containedByRange(const Ipv4Address& otherAddress, const Ipv4Address& otherMask) const
    {
        return Ipv4Address::maskedAddrAreEqual(otherAddress, address, otherMask) && (otherMask <= mask);
    }

    bool operator!=(Ipv4AddressRange other) const
    {
        return !operator==(other);
    }

    std::string str() const;
};

inline std::string Ipv4AddressRange::str() const
{
    std::string str(address.str(false));
    str += "/";
    str += mask.str(false);
    return str;
}

struct HostRouteParameters
{
    unsigned char ifIndex;
    Ipv4Address address;
    Metric linkCost;
};

typedef Ipv4Address RouterId;
typedef Ipv4Address AreaId;
typedef Ipv4Address LinkStateId;

struct LsaKeyType
{
    LinkStateId linkStateID;
    RouterId advertisingRouter;
};

class INET_API LsaKeyType_Less : public std::binary_function<LsaKeyType, LsaKeyType, bool>
{
  public:
    bool operator()(LsaKeyType leftKey, LsaKeyType rightKey) const;
};

struct DesignatedRouterId
{
    RouterId routerID;
    Ipv4Address ipInterfaceAddress;
};

const RouterId NULL_ROUTERID(0, 0, 0, 0);
const AreaId BACKBONE_AREAID(0, 0, 0, 0);
const LinkStateId NULL_LINKSTATEID(0, 0, 0, 0);
const Ipv4Address NULL_IPV4ADDRESS(0, 0, 0, 0);
const Ipv4AddressRange NULL_IPV4ADDRESSRANGE(Ipv4Address(0, 0, 0, 0), Ipv4Address(0, 0, 0, 0));
const DesignatedRouterId NULL_DESIGNATEDROUTERID = {
    Ipv4Address(0, 0, 0, 0), Ipv4Address(0, 0, 0, 0)
};

inline Ipv4Address operator&(Ipv4Address address, Ipv4Address mask)
{
    Ipv4Address maskedAddress;
    maskedAddress.set(address.getInt() & mask.getInt());
    return maskedAddress;
}

inline Ipv4Address operator|(Ipv4Address address, Ipv4Address match)
{
    Ipv4Address matchAddress;
    matchAddress.set(address.getInt() | match.getInt());
    return matchAddress;
}

inline bool isSameNetwork(Ipv4Address address1, Ipv4Address mask1, Ipv4Address address2, Ipv4Address mask2)
{
    return (mask1 == mask2) && ((address1 & mask1) == (address2 & mask2));
}

inline bool operator==(DesignatedRouterId leftID, DesignatedRouterId rightID)
{
    return leftID.routerID == rightID.routerID &&
           leftID.ipInterfaceAddress == rightID.ipInterfaceAddress;
}

inline bool operator!=(DesignatedRouterId leftID, DesignatedRouterId rightID)
{
    return !(leftID == rightID);
}

inline bool LsaKeyType_Less::operator()(LsaKeyType leftKey, LsaKeyType rightKey) const
{
    return (leftKey.linkStateID < rightKey.linkStateID) ||
           ((leftKey.linkStateID == rightKey.linkStateID) &&
            (leftKey.advertisingRouter < rightKey.advertisingRouter));
}

inline Ipv4Address ipv4AddressFromAddressString(const char *charForm)
{
    return L3AddressResolver().resolve(charForm, L3AddressResolver::ADDR_IPv4).toIpv4();
}

inline Ipv4Address ipv4NetmaskFromAddressString(const char *charForm)
{
    return L3AddressResolver().resolve(charForm, L3AddressResolver::ADDR_IPv4 | L3AddressResolver::ADDR_MASK).toIpv4();
}

inline char hexCharToByte(char hex)
{
    switch (hex) {
        case '0':
            return 0;

        case '1':
            return 1;

        case '2':
            return 2;

        case '3':
            return 3;

        case '4':
            return 4;

        case '5':
            return 5;

        case '6':
            return 6;

        case '7':
            return 7;

        case '8':
            return 8;

        case '9':
            return 9;

        case 'A':
            return 10;

        case 'B':
            return 11;

        case 'C':
            return 12;

        case 'D':
            return 13;

        case 'E':
            return 14;

        case 'F':
            return 15;

        case 'a':
            return 10;

        case 'b':
            return 11;

        case 'c':
            return 12;

        case 'd':
            return 13;

        case 'e':
            return 14;

        case 'f':
            return 15;

        default:
            break;
    }
    return 0;
}

inline char hexPairToByte(char upperHex, char lowerHex)
{
    return (hexCharToByte(upperHex) << 4) & (hexCharToByte(lowerHex));
}

} // namespace ospfv2

} // namespace inet

#endif
