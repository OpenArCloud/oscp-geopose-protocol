// Open AR Cloud GeoPoseProtocol C++ implementation
// Created based on the protocol definition:
// https://github.com/OpenArCloud/oscp-geopose-protocol

// Created by Gabor Soros, Nokia Bell Labs, 2022
// Copyright 2023 Nokia
// Licensed under the MIT License
// SPDX-License-Identifier: MIT


#ifndef _OSCP_GEOPOSE_PROTOCOL_JSON_H_
#define _OSCP_GEOPOSE_PROTOCOL_JSON_H_

#include <oscp-gpp/geoposeprotocol.h>

#include <nlohmann/json.hpp>
using json = nlohmann::json;

namespace oscp {

NLOHMANN_JSON_SERIALIZE_ENUM(SensorType, {
    {SensorType::CAMERA, "camera"},
    {SensorType::GEOLOCATION, "geolocation"},
    {SensorType::WIFI, "wifi"},
    {SensorType::BLUETOOTH, "bluetooth"},
    {SensorType::ACCELEROMETER, "accelerometer"},
    {SensorType::GYROSCOPE, "gyroscope"},
    {SensorType::MAGNETOMETER, "magnetometer"},
    {SensorType::UNKNOWN, "UNKNOWN"},
});

NLOHMANN_JSON_SERIALIZE_ENUM(ImageFormat, {
    {ImageFormat::RGBA32, "RGBA32"},
    {ImageFormat::GRAY8, "GRAY8"},
    {ImageFormat::DEPTH, "DEPTH"},
    {ImageFormat::JPG, "JPG"},
    {ImageFormat::UNKNOWN, "UNKNOWN"},
});

NLOHMANN_JSON_SERIALIZE_ENUM(CameraModel, {
    {CameraModel::SIMPLE_PINHOLE, "SIMPLE_PINHOLE"},
    {CameraModel::PINHOLE, "PINHOLE"},
    {CameraModel::SIMPLE_RADIAL, "SIMPLE_RADIAL"},
    {CameraModel::RADIAL, "RADIAL"},
    {CameraModel::OPENCV, "OPENCV"},
    {CameraModel::OPENCV_FISHEYE, "OPENCV_FISHEYE"},
    {CameraModel::FULL_OPENCV, "FULL_OPENCV"},
    {CameraModel::FOV, "FOV"},
    {CameraModel::SIMPLE_RADIAL_FISHEYE, "SIMPLE_RADIAL_FISHEYE"},
    {CameraModel::RADIAL_FISHEYE, "RADIAL_FISHEYE"},
    {CameraModel::THIN_PRISM_FISHEYE, "THIN_PRISM_FISHEYE"},
    {CameraModel::UNKNOWN, "UNKNOWN"}
});

void to_json(json& j, const Vector3& v);
void from_json(const json& j, Vector3& v);
void to_json(json& j, const ImageOrientation& t);
void from_json(const json& j, ImageOrientation& t);
void to_json(json& j, const CameraParameters& t);
void from_json(const json& j, CameraParameters& t);
void to_json(json& j, const Privacy& t);
void from_json(const json& j, Privacy& t);
void to_json(json& j, const CameraReading& t);
void from_json(const json& j, CameraReading& t);
void to_json(json& j, const GeolocationReading& t);
void from_json(const json& j, GeolocationReading& t);
void to_json(json& j, const WiFiReading& t);
void from_json(const json& j, WiFiReading& t);
void to_json(json& j, const BluetoothReading& t);
void from_json(const json& j, BluetoothReading& t);
void to_json(json& j, const AccelerometerReading& t);
void from_json(const json& j, AccelerometerReading& t);
void to_json(json& j, const GyroscopeReading& t);
void from_json(const json& j, GyroscopeReading& t);
void to_json(json& j, const MagnetometerReading& t);
void from_json(const json& j, MagnetometerReading& t);
void to_json(json& j, const Sensor& t);
void from_json(const json& j, Sensor& t);
void to_json(json& j, const SensorReadings& t);
void from_json(const json& j, SensorReadings& t);
void to_json(json& j, const GeoPoseAccuracy& t);
void from_json(const json& j, GeoPoseAccuracy& t);
void to_json(json& j, const GeoPoseResponse& t);
void from_json(const json& j, GeoPoseResponse& t);
void to_json(json& j, const GeoPoseRequest& t);
void from_json(const json& j, GeoPoseRequest& t);

} // namespace oscp

#endif // _OSCP_GEOPOSE_PROTOCOL_JSON_H_
