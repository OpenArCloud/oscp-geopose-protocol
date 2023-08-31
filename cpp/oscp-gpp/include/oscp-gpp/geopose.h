// Open AR Cloud GeoPoseProtocol C++ implementation
// Created based on the protocol definition:
// https://github.com/OpenArCloud/oscp-geopose-protocol

// Created by Gabor Soros, Nokia Bell Labs, 2022
// Copyright 2023 Nokia
// Licensed under the MIT License
// SPDX-License-Identifier: MIT


#ifndef _OSCP_GEOPOSE_H_
#define _OSCP_GEOPOSE_H_

#include <string>

namespace oscp {

struct Position {
    double lon = 0.0;
    double lat = 0.0;
    double h = 0.0;
};

struct Quaternion {
    double x = 0.0;
    double y = 0.0;
    double z = 0.0;
    double w = 1.0;
};

struct GeoPose {
    Position position;
    Quaternion quaternion;
};

} // namespace oscp

#endif // _OSCP_GEOPOSE_H_
