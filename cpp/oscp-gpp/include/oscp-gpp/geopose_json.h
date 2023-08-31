// Open AR Cloud GeoPoseProtocol C++ implementation
// Created based on the protocol definition:
// https://github.com/OpenArCloud/oscp-geopose-protocol

// Created by Gabor Soros, Nokia Bell Labs, 2022
// Copyright 2023 Nokia
// Licensed under the MIT License
// SPDX-License-Identifier: MIT


#ifndef _OSCP_GEOPOSE_JSON_H_
#define _OSCP_GEOPOSE_JSON_H_

#include <oscp-gpp/geopose.h>

#include <nlohmann/json.hpp>
using json = nlohmann::json;

namespace oscp {

void to_json(json& j, const oscp::Position& p);
void from_json(const json& j, struct Position& p);
void to_json(json& j, const Quaternion& o);
void from_json(const json& j, struct Quaternion& o);
void to_json(json& j, const GeoPose& geoPose);
void from_json(const json& j, GeoPose& geoPose);

} // namespace oscp

#endif // _OSCP_GEOPOSE_JSON_H_

