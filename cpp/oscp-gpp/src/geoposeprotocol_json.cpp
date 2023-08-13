// Open AR Cloud GeoPoseProtocol C++ implementation
// Created based on the protocol definition:
// https://github.com/OpenArCloud/oscp-geopose-protocol

// Created by Gabor Soros, Nokia Bell Labs, 2022
// Copyright 2023 Nokia
// Licensed under the MIT License
// SPDX-License-Identifier: MIT


#include <oscp-gpp/geoposeprotocol_json.h>
#include <oscp-gpp/geopose_json.h>
#include <iostream>

namespace oscp {

void to_json(json& j, const Vector3& v) {
    j = json{
        {"x", v.x},
        {"y", v.y},
        {"z", v.z}
    };
}

void from_json(const json& j, Vector3& v) {
    j.at("x").get_to(v.x);
    j.at("y").get_to(v.y);
    j.at("z").get_to(v.z);
}

void to_json(json& j, const ImageOrientation& t) {
    j = json{
        {"mirrored", t.mirrored},
        {"rotation", t.rotation}
    };
}

void from_json(const json& j, ImageOrientation& t) {
    j.at("mirrored").get_to(t.mirrored);
    j.at("rotation").get_to(t.rotation);
}

void to_json(json& j, const CameraParameters& t) {
    if (t.model != CameraModel::UNKNOWN) {
        j["model"] = t.model;
    }
    if (!t.modelParams.empty()) {
        j["modelParams"] = t.modelParams;
    }
    if (!t.minMaxDepth.empty()) {
        j["minMaxDepth"], t.minMaxDepth;
    }
    if (!t.minMaxDisparity.empty()) {
        j["minMaxDisparity"], t.minMaxDisparity;
    }
}

void from_json(const json& j, CameraParameters& t) {
    if (j.find("model") != j.end()) {
        t.model = cameraModelFromString(j.at("model"));
    }
    if (j.find("modelParams") != j.end()) {
        j.at("modelParams").get_to(t.modelParams);
    }
    if (j.find("minMaxDepth") != j.end()) {
        j.at("minMaxDepth").get_to(t.minMaxDepth);
    }
    if (j.find("minMaxDisparity") != j.end()) {
        j.at("minMaxDisparity").get_to(t.minMaxDisparity);
    }
}

void to_json(json& j, const Privacy& t) {
    j = json{
        {"dataRetention", t.dataRetention},
        {"dataAcceptableUse", t.dataAcceptableUse},
        {"dataSanitizationApplied", t.dataSanitizationApplied},
        {"dataSanitizationRequested", t.dataSanitizationRequested}
    };
}

void from_json(const json& j, Privacy& t) {
    j.at("dataRetention").get_to(t.dataRetention);
    j.at("dataAcceptableUse").get_to(t.dataAcceptableUse);
    j.at("dataSanitizationApplied").get_to(t.dataSanitizationApplied);
    j.at("dataSanitizationRequested").get_to(t.dataSanitizationRequested);
}

void to_json(json& j, const CameraReading& t) {
    j = json{
        {"timestamp", t.timestamp},
        {"sensorId", t.sensorId},
        {"privacy", t.privacy},
        {"sequenceNumber", t.sequenceNumber},
        {"imageFormat", t.imageFormat},
        {"size", t.size},
        {"imageBytes", t.imageBytes},
        {"imageOrientation", t.imageOrientation},
        {"params", t.params},
    };
}

void from_json(const json& j, CameraReading& t) {
    j.at("timestamp").get_to(t.timestamp);
    j.at("sensorId").get_to(t.sensorId);
    j.at("privacy").get_to(t.privacy);
    j.at("sequenceNumber").get_to(t.sequenceNumber);
    t.imageFormat = imageFormatFromString(j.at("imageFormat"));
    j.at("size").get_to(t.size);
    j.at("imageBytes").get_to(t.imageBytes);
    if (j.find("imageOrientation") != j.end()) {
        j.at("imageOrientation").get_to(t.imageOrientation);
    }
    if (j.find("params") != j.end()) {
        j.at("params").get_to(t.params);
    }
}

void to_json(json& j, const GeolocationReading& t) {
    j = json{
        {"timestamp", t.timestamp},
        {"sensorId", t.sensorId},
        {"privacy", t.privacy},
        {"latitude", t.latitude},
        {"longitude", t.longitude},
        {"altitude", t.altitude},
        {"accuracy", t.accuracy},
        {"altitudeAccuracy", t.altitudeAccuracy},
        {"heading", t.heading},
        {"speed", t.speed}
    };
}

void from_json(const json& j, GeolocationReading& t) {
    j.at("timestamp").get_to(t.timestamp);
    j.at("sensorId").get_to(t.sensorId);
    j.at("privacy").get_to(t.privacy);
    j.at("latitude").get_to(t.latitude);
    j.at("longitude").get_to(t.longitude);
    if (j.find("altitude") != j.end()) {
        j.at("altitude").get_to(t.altitude);
    }
    if (j.find("accuracy") != j.end()) {
        j.at("accuracy").get_to(t.accuracy);
    }
    if (j.find("altitudeAccuracy") != j.end()) {
        j.at("altitudeAccuracy").get_to(t.altitudeAccuracy);
    }
    if (j.find("heading") != j.end()) {
        j.at("heading").get_to(t.heading);
    }
    if (j.find("speed") != j.end()) {
        j.at("speed").get_to(t.speed);
    }
}

void to_json(json& j, const WiFiReading& t) {
    j = json{
        {"timestamp", t.timestamp},
        {"sensorId", t.sensorId},
        {"privacy", t.privacy},
        {"BSSID", t.BSSID},
        {"frequency", t.frequency},
        {"RSSI", t.RSSI},
        {"SSID", t.SSID},
        {"scanTimeStart", t.scanTimeStart},
        {"scanTimeEnd", t.scanTimeEnd},
    };
}

void from_json(const json& j, WiFiReading& t) {
    j.at("timestamp").get_to(t.timestamp);
    j.at("sensorId").get_to(t.sensorId);
    j.at("privacy").get_to(t.privacy);
    j.at("BSSID").get_to(t.BSSID);
    j.at("frequency").get_to(t.frequency);
    j.at("RSSI").get_to(t.RSSI);
    j.at("SSID").get_to(t.SSID);
    j.at("scanTimeStart").get_to(t.scanTimeStart);
    j.at("scanTimeEnd").get_to(t.scanTimeEnd);
}

void to_json(json& j, const BluetoothReading& t) {
    j = json{
        {"timestamp", t.timestamp},
        {"sensorId", t.sensorId},
        {"privacy", t.privacy},
        {"address", t.address},
        {"RSSI", t.RSSI},
        {"name", t.name},
    };
}

void from_json(const json& j, BluetoothReading& t) {
    j.at("timestamp").get_to(t.timestamp);
    j.at("sensorId").get_to(t.sensorId);
    j.at("privacy").get_to(t.privacy);
    j.at("address").get_to(t.address);
    j.at("RSSI").get_to(t.RSSI);
    j.at("name").get_to(t.name);
}

void to_json(json& j, const AccelerometerReading& t) {
    j = json{
        {"timestamp", t.timestamp},
        {"sensorId", t.sensorId},
        {"privacy", t.privacy},
        {"x", t.x},
        {"y", t.y},
        {"z", t.z}
    };
}

void from_json(const json& j, AccelerometerReading& t) {
    j.at("timestamp").get_to(t.timestamp);
    j.at("sensorId").get_to(t.sensorId);
    j.at("privacy").get_to(t.privacy);
    j.at("x").get_to(t.x);
    j.at("y").get_to(t.y);
    j.at("z").get_to(t.z);
}

void to_json(json& j, const GyroscopeReading& t) {
    j = json{
        {"timestamp", t.timestamp},
        {"sensorId", t.sensorId},
        {"privacy", t.privacy},
        {"x", t.x},
        {"y", t.y},
        {"z", t.z}
    };
}

void from_json(const json &j, GyroscopeReading &t) {
    j.at("timestamp").get_to(t.timestamp);
    j.at("sensorId").get_to(t.sensorId);
    j.at("privacy").get_to(t.privacy);
    j.at("x").get_to(t.x);
    j.at("y").get_to(t.y);
    j.at("z").get_to(t.z);
}

void to_json(json& j, const MagnetometerReading& t) {
    j = json{
        {"timestamp", t.timestamp},
        {"sensorId", t.sensorId},
        {"privacy", t.privacy},
        {"x", t.x},
        {"y", t.y},
        {"z", t.z}
    };
}

void from_json(const json& j, MagnetometerReading& t) {
    j.at("timestamp").get_to(t.timestamp);
    j.at("sensorId").get_to(t.sensorId);
    j.at("privacy").get_to(t.privacy);
    j.at("x").get_to(t.x);
    j.at("y").get_to(t.y);
    j.at("z").get_to(t.z);
}

void to_json(json& j, const Sensor& t) {
    j = json{
        {"type", t.type},
        {"id", t.id},
    };
    if (!t.name.empty()) {
        j["name"] = t.name;
    }
    if (!t.model.empty()) {
        j["model"] = t.model;
    }
    if (!t.rigIdentifier.empty()) {
        j["rigIdentifier"] = t.rigIdentifier;
        j["rigRotation"] = t.rigRotation;
        j["rigTranslation"] = t.rigTranslation;
    }
}

void from_json(const json& j, Sensor& t) {
    t.type = sensorTypefromString(j.at("type"));
    j.at("id").get_to(t.id);
    if (j.find("name") != j.end()) {
        j.at("name").get_to(t.name);
    }
    if (j.find("model") != j.end()) {
        j.at("model").get_to(t.model);
    }
    if (j.find("rigIdentifier") != j.end()) {
        j.at("rigIdentifier").get_to(t.rigIdentifier);
    }
    if (j.find("rigRotation") != j.end()) {
        j.at("rigRotation").get_to(t.rigRotation);
    }
    if (j.find("rigTranslation") != j.end()) {
        j.at("rigTranslation").get_to(t.rigTranslation);
    }
}

void to_json(json& j, const SensorReadings& t) {
    j = json{};
    if (!t.cameraReadings.empty())
        j["cameraReadings"] = t.cameraReadings;
    if (!t.geolocationReadings.empty())
        j["geolocationReadings"] = t.geolocationReadings;
    if (!t.wifiReadings.empty())
        j["wifiReadings"] = t.wifiReadings;
    if (!t.bluetoothReadings.empty())
        j["bluetoothReadings"] = t.bluetoothReadings;
    if (!t.accelerometerReadings.empty())
        j["accelerometerReadings"] = t.accelerometerReadings;
    if (!t.gyroscopeReadings.empty())
        j["gyroscopeReadings"] = t.gyroscopeReadings;
    if (!t.magnetometerReadings.empty())
        j["magnetometerReadings"] = t.magnetometerReadings;
}

void from_json(const json& j, SensorReadings& t) {
    if (j.find("cameraReadings") != j.end()) {
        j.at("cameraReadings").get_to(t.cameraReadings);
    }
    if (j.find("geolocationReadings") != j.end()) {
        j.at("geolocationReadings").get_to(t.geolocationReadings);
    }
    if (j.find("wifiReadings") != j.end()) {
        j.at("wifiReadings").get_to(t.wifiReadings);
    }
    if (j.find("bluetoothReadings") != j.end()) {
        j.at("bluetoothReadings").get_to(t.bluetoothReadings);
    }
    if (j.find("accelerometerReadings") != j.end()) {
        j.at("accelerometerReadings").get_to(t.accelerometerReadings);
    }
    if (j.find("gyroscopeReadings") != j.end()) {
        j.at("gyroscopeReadings").get_to(t.gyroscopeReadings);
    }
    if (j.find("magnetometerReadings") != j.end()) {
        j.at("magnetometerReadings").get_to(t.magnetometerReadings);
    }
}

void to_json(json& j, const GeoPoseAccuracy& t) {
    j = json{
        {"position", t.position},
        {"orientation", t.orientation},
    };
}

void from_json(const json& j, GeoPoseAccuracy& t) {
    j.at("position").get_to(t.position);
    j.at("orientation").get_to(t.orientation);
}

void to_json(json& j, const GeoPoseResponse& t) {
    j = json{
        {"type", t.type},
        {"id", t.id},
        {"timestamp", t.timestamp},
        {"accuracy", t.accuracy},
        {"geopose", t.geopose}
    };
}

void from_json(const json& j, GeoPoseResponse& t) {
    j.at("type").get_to(t.type);
    j.at("id").get_to(t.id);
    j.at("timestamp").get_to(t.timestamp);
    j.at("accuracy").get_to(t.accuracy);
    j.at("geopose").get_to(t.geopose);
}

void to_json(json& j, const GeoPoseRequest& t) {
    j = json{
        {"type", t.type},
        {"id", t.id},
        {"timestamp", t.timestamp},
        {"sensors", t.sensors},
        {"sensorReadings", t.sensorReadings},
    };
    if (!t.priorPoses.empty()) {
        j["priorPoses"] = t.priorPoses;
    }
}

void from_json(const json& j, GeoPoseRequest& t) {
    j.at("type").get_to(t.type);
    j.at("id").get_to(t.id);
    j.at("timestamp").get_to(t.timestamp);
    j.at("sensors").get_to(t.sensors);
    j.at("sensorReadings").get_to(t.sensorReadings);
    if (j.find("priorPoses") != j.end()) {
        j.at("priorPoses").get_to(t.priorPoses);
    }
}

} // namespace oscp
