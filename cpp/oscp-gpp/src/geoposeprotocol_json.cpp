// Open AR Cloud GeoPoseProtocol C++ implementation
// Created based on the protocol definition:
// https://github.com/OpenArCloud/oscp-geopose-protocol

// Created by Gabor Soros, Nokia Bell Labs, 2022
// Copyright Nokia
// MIT License


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
        j.at("model").get_to(t.model);
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

void to_json(json& j, const AbstractSensorReading& t) {
    j = json{};
}

void from_json(const json& j, AbstractSensorReading& t) {
}


void to_json(json& j, const CameraReading& t) {
    j = json{
        {"sequenceNumber", t.sequenceNumber},
        {"imageFormat", t.imageFormat},
        {"size", t.size},
        {"imageBytes", t.imageBytes},
        {"imageOrientation", t.imageOrientation}
    };
}

void from_json(const json& j, CameraReading& t) {
    j.at("sequenceNumber").get_to(t.sequenceNumber);
    j.at("imageFormat").get_to(t.imageFormat);
    j.at("size").get_to(t.size);
    j.at("imageBytes").get_to(t.imageBytes);
    if (j.find("imageOrientation") != j.end()) {
        j.at("imageOrientation").get_to(t.imageOrientation);
    }
}

void to_json(json& j, const GeolocationReading& t) {
    j = json{
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
    j.at("latitude").get_to(t.latitude);
    j.at("longitude").get_to(t.longitude);
    j.at("altitude").get_to(t.altitude);
    j.at("accuracy").get_to(t.accuracy);
    j.at("altitudeAccuracy").get_to(t.altitudeAccuracy);
    j.at("heading").get_to(t.heading);
    j.at("speed").get_to(t.speed);
}

void to_json(json& j, const WiFiReading& t) {
    j = json{
        {"BSSID", t.BSSID},
        {"frequency", t.frequency},
        {"RSSI", t.RSSI},
        {"SSID", t.SSID},
        {"scanTimeStart", t.scanTimeStart},
        {"scanTimeEnd", t.scanTimeEnd},
    };
}

void from_json(const json& j, WiFiReading& t) {
    j.at("BSSID").get_to(t.BSSID);
    j.at("frequency").get_to(t.frequency);
    j.at("RSSI").get_to(t.RSSI);
    j.at("SSID").get_to(t.SSID);
    j.at("scanTimeStart").get_to(t.scanTimeStart);
    j.at("scanTimeEnd").get_to(t.scanTimeEnd);
}

void to_json(json& j, const BluetoothReading& t) {
    j = json{
        {"address", t.address},
        {"RSSI", t.RSSI},
        {"name", t.name},
    };
}

void from_json(const json& j, BluetoothReading& t) {
    j.at("address").get_to(t.address);
    j.at("RSSI").get_to(t.RSSI);
    j.at("name").get_to(t.name);
}

void to_json(json& j, const AccelerometerReading& t) {
    j = json{
        {"x", t.x},
        {"y", t.y},
        {"z", t.z}
    };
}

void from_json(const json& j, AccelerometerReading& t) {
    j.at("x").get_to(t.x);
    j.at("y").get_to(t.y);
    j.at("z").get_to(t.z);
}

void to_json(json& j, const GyroscopeReading& t) {
    j = json{
        {"x", t.x},
        {"y", t.y},
        {"z", t.z}
    };
}

void from_json(const json& j, GyroscopeReading& t) {
    j.at("x").get_to(t.x);
    j.at("y").get_to(t.y);
    j.at("z").get_to(t.z);
}

void to_json(json& j, const MagnetometerReading& t) {
    j = json{
        {"x", t.x},
        {"y", t.y},
        {"z", t.z}
    };
}

void from_json(const json& j, MagnetometerReading& t) {
    j.at("x").get_to(t.x);
    j.at("y").get_to(t.y);
    j.at("z").get_to(t.z);
}

void to_json(json& j, const AbstractSensorParameters& t) {
    j = json{};
}

void from_json(const json& j, AbstractSensorParameters& t) {
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

    switch (t.type) {
    case SensorType::CAMERA:
        j["params"] = t.cameraParams; break;
    //TODO: add other types here (but if possible move params into CameraSensorReading)
    default:
        std::cout << "WARNING: unexpected sensor type seen while converting Sensor to JSON" << std::endl;
        break;
    }
}

void from_json(const json& j, Sensor& t) {
    j.at("type").get_to(t.type);
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

    if (j.find("params") != j.end()) {
        // NOTE: the type switch below was added by Gabor to be able to parse it into correct type
        switch (t.type) {
        case SensorType::CAMERA:
            j.at("params").get_to(t.cameraParams); break;
        //TODO: add other types here. But I think the cameraParams should be moved from Sensor to CameraReading
        default:
            std::cout << "WARNING: unexpected sensor type seen while converting JSON to Sensor" << std::endl;
            break;
        }
    }
}

void to_json(json& j, const SensorReading& t) {
    j = json{
        {"timestamp", t.timestamp},
        {"sensorId", t.sensorId},
        {"privacy", t.privacy},
    };

    // TODO: sensorType was added by Gabor to be able to decide how to parse it below
    j["sensorType"] = t.sensorType;

    switch (t.sensorType) {
    case SensorType::CAMERA:
        j["reading"] = t.cameraReading; break;
    case SensorType::GEOLOCATION:
        j["reading"] = t.geolocationReading; break;
    case SensorType::WIFI:
        j["reading"] = t.wifiReading; break;
    case SensorType::BLUETOOTH:
        j["reading"] = t.bluetoothReading; break;
    case SensorType::ACCELEROMETER:
        j["reading"] = t.accelerometerReading; break;
    case SensorType::GYROSCOPE:
        j["reading"] = t.gyroscopeReading; break;
    case SensorType::MAGNETOMETER:
        j["reading"] = t.magnetometerReading; break;
    default:
        std::cout << "WARNING: unexpected sensor type seen while converting SensorReading to JSON" << std::endl;
        break;
    }
}

void from_json(const json& j, SensorReading& t) {
    j.at("timestamp").get_to(t.timestamp);
    j.at("sensorId").get_to(t.sensorId);
    j.at("privacy").get_to(t.privacy);

    // TODO: the sensorType key was added by Gabor to be able to decide how to parse it
    if (j.find("sensorType") == j.end()) {
        std::string errorMessage = "WARNING: no sensorType in the SensorReading";
        std::cout << errorMessage << std::endl;
        throw std::invalid_argument(errorMessage);
    }
    j.at("sensorType").get_to(t.sensorType);

    switch (t.sensorType)
    {
    case SensorType::CAMERA:
        j.at("reading").get_to(t.cameraReading); break;
    case SensorType::GEOLOCATION:
        j.at("reading").get_to(t.geolocationReading); break;
    case SensorType::WIFI:
        j.at("reading").get_to(t.wifiReading); break;
    case SensorType::BLUETOOTH:
        j.at("reading").get_to(t.bluetoothReading); break;
    case SensorType::ACCELEROMETER:
        j.at("reading").get_to(t.accelerometerReading); break;
    case SensorType::GYROSCOPE:
        j.at("reading").get_to(t.gyroscopeReading); break;
    case SensorType::MAGNETOMETER:
        j.at("reading").get_to(t.magnetometerReading); break;
    default:
        std::cout << "WARNING: unexpected sensor type seen while converting JSON to SensorReading" << std::endl;
        break;
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
