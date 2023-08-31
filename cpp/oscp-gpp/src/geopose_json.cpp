// Open AR Cloud GeoPoseProtocol C++ implementation
// Created based on the protocol definition:
// https://github.com/OpenArCloud/oscp-geopose-protocol

// Created by Gabor Soros, Nokia Bell Labs, 2022
// Copyright 2023 Nokia
// Licensed under the MIT License
// SPDX-License-Identifier: MIT


#include <oscp-gpp/geopose_json.h>

namespace oscp {

void to_json(json& j, const oscp::Position& p) {
    j = json{
        {"lat", p.lat},
        {"lon", p.lon},
        {"h", p.h}
    };
}

void from_json(const json& j, struct Position& p) {
    j.at("lat").get_to(p.lat);
    j.at("lon").get_to(p.lon);
    j.at("h").get_to(p.h);
}

void to_json(json& j, const Quaternion& o) {
    j = json{
        {"x", o.x},
        {"y", o.y},
        {"z", o.z},
        {"w", o.w}
    };
}

void from_json(const json& j, struct Quaternion& o) {
    j.at("x").get_to(o.x);
    j.at("y").get_to(o.y);
    j.at("z").get_to(o.z);
    j.at("w").get_to(o.w);
}

void to_json(json& j, const GeoPose& geoPose) {
    j = json{
        {"position", geoPose.position},
        {"quaternion", geoPose.quaternion}
    };
}

void from_json(const json& j, GeoPose& geoPose) {
    j.at("quaternion").get_to(geoPose.quaternion);
    j.at("position").get_to(geoPose.position);
}

} // namespace oscp
