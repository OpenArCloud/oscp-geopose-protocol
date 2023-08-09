// Open AR Cloud GeoPoseProtocol C++ implementation
// Created based on the protocol definition:
// https://github.com/OpenArCloud/oscp-geopose-protocol
// and the JavaScript implementation:
// https://github.com/OpenArCloud/gpp-access/

// Created by Gabor Soros, Nokia Bell Labs, 2022
// Copyright 2023 Nokia
// Licensed under the MIT License
// SPDX-License-Identifier: MIT


#ifndef _OSCP_GEOPOSE_PROTOCOL_H_
#define _OSCP_GEOPOSE_PROTOCOL_H_

#include <oscp-gpp/geopose.h>

#include <chrono>
#include <string>
#include <vector>
#include <stdexcept>

namespace oscp {

/**
Sensor types usable with the GeoPose protocol
Use when creating a new Sensor object.
*/
enum class SensorType {
    CAMERA,
    GEOLOCATION,
    WIFI,
    BLUETOOTH,
    ACCELEROMETER,
    GYROSCOPE,
    MAGNETOMETER,
    UNKNOWN
};
// TODO: add altitude sensor

inline std::string toString(const enum oscp::SensorType sensorType) {
    switch(sensorType) {
        case SensorType::CAMERA:
            return "camera";
        case SensorType::GEOLOCATION:
            return "geolocation";
        case SensorType::WIFI:
            return "wifi";
        case SensorType::BLUETOOTH:
            return "bluetooth";
        case SensorType::ACCELEROMETER:
            return "accelerometer";
        case SensorType::GYROSCOPE:
            return "gyroscope";
        case SensorType::MAGNETOMETER:
            return "magnetometer";
        default:
            throw std::runtime_error("Unknown sensor type");
    }
}

inline std::ostream & operator<< (std::ostream &out, oscp::SensorType const &sensorType) {
    out << toString(sensorType);
    return out;
}

inline enum SensorType sensorTypefromString(const std::string& str) {
    if (str.compare("camera")==0) {
        return SensorType::CAMERA;
    } else if (str.compare("geolocation")==0) {
        return SensorType::GEOLOCATION;
    } else if (str.compare("wifi")==0) {
        return SensorType::WIFI;
    } else if (str.compare("bluetooth")==0) {
        return SensorType::BLUETOOTH;
    } else if (str.compare("accelerometer")==0) {
        return SensorType::ACCELEROMETER;
    } else if (str.compare("gyroscope")==0) {
        return SensorType::GYROSCOPE;
    } else if (str.compare("magnetometer")==0) {
        return SensorType::MAGNETOMETER;
    } else {
        throw std::runtime_error("Unknown sensor type: " + str);
    }
}


/**
Image formats usable with the CameraReading object
Use when creating a SensorReading object for a new Sensor of type 'camera'.
*/
enum class ImageFormat {
    RGBA32,
    GRAY8,
    DEPTH,
    JPG,
    UNKNOWN
};

inline std::string toString(const enum oscp::ImageFormat imageFormat) {
    switch(imageFormat) {
        case ImageFormat::RGBA32:
            return "RGBA32";
        case ImageFormat::GRAY8:
            return "GRAY8";
        case ImageFormat::DEPTH:
            return "DEPTH";
        case ImageFormat::JPG:
            return "JPG";
        default:
            throw std::runtime_error("Unknown image format");
    }
}

inline std::ostream & operator<< (std::ostream &out, oscp::ImageFormat const &imageFormat) {
    out << toString(imageFormat);
    return out;
}

inline enum ImageFormat imageFormatFromString(const std::string& str) {
    if (str.compare("RGBA32")==0) {
        return ImageFormat::RGBA32;
    } else if (str.compare("GRAY8")==0) {
        return ImageFormat::GRAY8;
    } else if (str.compare("DEPTH")==0) {
        return ImageFormat::DEPTH;
    } else if (str.compare("JPG")==0) {
        return ImageFormat::JPG;
    } else {
        throw std::runtime_error("Unknown image format: " + str);
    }
}

/**
Image orientations usable with the CameraReading object
*/
struct ImageOrientation {
    bool mirrored = false; // Value as provided from Camera sensor
    float rotation = 0.0f; // Value as provided from Camera sensor

    ImageOrientation(bool mirrored = false, float rotation = 0.0f) {
        this->mirrored = mirrored;
        this->rotation = rotation;
    }
};

inline std::ostream & operator<< (std::ostream &out, oscp::ImageOrientation const &t) {
    out << "{"
        << "mirrored" << ":" << t.mirrored << ","
        << "rotation" << ":" << t.rotation
        << "}";
    return out;
}

inline std::string toString(const oscp::ImageOrientation &t) {
    return std::string("{") + \
        std::string("mirrored:") + std::to_string(t.mirrored) + std::string(",")+ \
        std::string("rotation:") + std::to_string(t.rotation) + \
    std::string("}");
}

/**
* The camera models of Colmap are used here
* See https://colmap.github.io/cameras.html
*/
enum class CameraModel {
    SIMPLE_PINHOLE, // f, cx, cy
    PINHOLE, // fx, fy, cx, cy
    SIMPLE_RADIAL, // f, cx, cy, k
    RADIAL, // f, cx, cy, k1, k2
    OPENCV, // fx, fy, cx, cy, k1, k2, p1, p2
    OPENCV_FISHEYE, // fx, fy, cx, cy, k1, k2, k3, k4
    FULL_OPENCV, // fx, fy, cx, cy, k1, k2, p1, p2, k3, k4, k5, k6
    FOV, // fx, fy, cx, cy, omega
    SIMPLE_RADIAL_FISHEYE, // f, cx, cy, k
    RADIAL_FISHEYE, // f, cx, cy, k1, k2
    THIN_PRISM_FISHEYE, // fx, fy, cx, cy, k1, k2, p1, p2, k3, k4, sx1, sy1
    UNKNOWN
};

inline std::string toString(const enum oscp::CameraModel cameraModel) {
    switch(cameraModel) {
        case CameraModel::SIMPLE_PINHOLE:
            return "SIMPLE_PINHOLE";
        case CameraModel::PINHOLE:
            return "PINHOLE";
        case CameraModel::SIMPLE_RADIAL:
            return "SIMPLE_RADIAL";
        case CameraModel::RADIAL:
            return "RADIAL";
        case CameraModel::OPENCV:
            return "OPENCV";
        case CameraModel::OPENCV_FISHEYE:
            return "OPENCV_FISHEYE";
        case CameraModel::FULL_OPENCV:
            return "FULL_OPENCV";
        case CameraModel::FOV:
            return "FOV";
        case CameraModel::SIMPLE_RADIAL_FISHEYE:
            return "SIMPLE_RADIAL_FISHEYE";
        case CameraModel::RADIAL_FISHEYE:
            return "RADIAL_FISHEYE";
        case CameraModel::THIN_PRISM_FISHEYE:
            return "THIN_PRISM_FISHEYE";
        default:
            throw std::runtime_error("Unknown camera model");
    }
}

inline std::ostream & operator<< (std::ostream &out, oscp::CameraModel const &cameraModel) {
    out << toString(cameraModel);
    return out;
}

inline enum CameraModel cameraModelFromString(const std::string& str) {
    if (str.compare("SIMPLE_PINHOLE")==0) {
        return CameraModel::SIMPLE_PINHOLE;
    } else if (str.compare("PINHOLE")==0) {
        return CameraModel::PINHOLE;
    } else if (str.compare("SIMPLE_RADIAL")==0) {
        return CameraModel::SIMPLE_RADIAL;
    } else if (str.compare("RADIAL")==0) {
        return CameraModel::RADIAL;
    } else if (str.compare("OPENCV")==0) {
        return CameraModel::OPENCV;
    } else if (str.compare("OPENCV_FISHEYE")==0) {
        return CameraModel::OPENCV_FISHEYE;
    } else if (str.compare("FULL_OPENCV")==0) {
        return CameraModel::FULL_OPENCV;
    } else if (str.compare("FOV")==0) {
        return CameraModel::FOV;
    } else if (str.compare("SIMPLE_RADIAL_FISHEYE")==0) {
        return CameraModel::SIMPLE_RADIAL_FISHEYE;
    } else if (str.compare("RADIAL_FISHEYE")==0) {
        return CameraModel::RADIAL_FISHEYE;
    } else if (str.compare("THIN_PRISM_FISHEYE")==0) {
        return CameraModel::THIN_PRISM_FISHEYE;
    } else {
        throw std::runtime_error("Unknown camera model: " + str);
    }
}

struct Vector3 {
    float x = 0.0f;
    float y = 0.0f;
    float z = 0.0f;
};

struct CameraParameters {
    enum CameraModel model = CameraModel::UNKNOWN; // [optional] // TODO: std::string in the v1 standard, but enum is better suited here
    std::vector<float> modelParams; // [optional]
    std::vector<float> minMaxDepth; // [optional] // for depth image
    std::vector<float> minMaxDisparity; // [optional] // for disparity image
};

struct Privacy {
    std::vector<std::string> dataRetention; //acceptable policies for server-side data retention
    std::vector<std::string> dataAcceptableUse; //acceptable policies for server-side data use
    std::vector<std::string> dataSanitizationApplied; //client-side data sanitization applied
    std::vector<std::string> dataSanitizationRequested; //server-side data sanitization requested
};

struct BaseSensorReading {
    std::time_t timestamp; // The number of milliseconds since the Unix Epoch.
    std::string sensorId;
    struct Privacy privacy = Privacy();

    //SensorType _sensorType = SensorType::UNKNOWN;
    // added by Gabor to be able to determine how to parse a received SensorReading
    // Currently an AccelerometerReading, a GyroscopeReading, and a MagnetometerReading would look exactly the same!
    // We could find out the sensorType via Sensors[sensorId] but there is no guarantee that the Sensors were parsed already and stored anywhere.
};

struct CameraReading : public BaseSensorReading {
    size_t sequenceNumber = 0;
    enum ImageFormat imageFormat = ImageFormat::UNKNOWN; // TODO: string
    size_t size[2] = {0,0}; // width, height
    std::string imageBytes; // base64 encoded image data
    struct ImageOrientation imageOrientation = ImageOrientation(); // [optional]

    CameraParameters params;
    // Note: as the intrinsics can change over time, it is better to store the cameraParams in the CameraReading per frame
    // and not in the Sensor description once. This change was implemented from GPPv1 to GPPv2

    //CameraReading() {
    //    _sensorType = SensorType::CAMERA;
    //}
};

//aligns with https://w3c.github.io/geolocation-sensor/
struct GeolocationReading : BaseSensorReading {
    float latitude = 0.0f;
    float longitude = 0.0f;
    float altitude = 0.0f;
    float accuracy = 0.0f;
    float altitudeAccuracy = 0.0f;
    float heading = 0.0f;
    float speed = 0.0f;
};

struct WiFiReading : BaseSensorReading {
    std::string BSSID;
    float frequency = 0.0f; // TODO: shouldn't this be a band frequency range?
    float RSSI = 0.0f; // TODO: shouldn't this be a vector?
    std::string SSID;
    std::time_t scanTimeStart;  // The number of milliseconds since the Unix Epoch.
    std::time_t scanTimeEnd;  // The number of milliseconds since the Unix Epoch.
};

struct BluetoothReading : BaseSensorReading {
    std::string address;
    float RSSI = 0.0f; // TODO: shouldn't this be a vector?
    std::string name;
};

struct AccelerometerReading : BaseSensorReading {
    float x = 0.0f;
    float y = 0.0f;
    float z = 0.0f;
};

struct GyroscopeReading : BaseSensorReading {
    float x = 0.0f;
    float y = 0.0f;
    float z = 0.0f;
};

struct MagnetometerReading : BaseSensorReading {
    float x = 0.0f;
    float y = 0.0f;
    float z = 0.0f;
};

struct Sensor {
    SensorType type = SensorType::UNKNOWN; //std::string type; // camera, geolocation, wifi, bluetooth, accelerometer, gyroscope, magnetometer
    std::string id;
    std::string name; // [optional]
    std::string model; // [optional] // TODO: is this CameraModel or other model? If CameraModel, it is redundant here and should be inside params only.
    std::string rigIdentifier; // [optional]
    struct Quaternion rigRotation; // [optional] // rotation quaternion from rig to sensor
    struct Vector3 rigTranslation; // [optional] //  translation vector from rig to sensor
};

struct SensorReadings {
    std::vector<CameraReading> cameraReadings;
    std::vector<AccelerometerReading> accelerometerReadings;
    std::vector<GeolocationReading> geolocationReadings;
    std::vector<WiFiReading> wifiReadings;
    std::vector<BluetoothReading> bluetoothReadings;
    std::vector<GyroscopeReading> gyroscopeReadings;
    std::vector<MagnetometerReading> magnetometerReadings;
};

struct GeoPoseAccuracy {
    float position = std::numeric_limits<float>::max(); //  mean for all components in meters
    float orientation = std::numeric_limits<float>::max(); // mean for all 3 angles in degrees
};

struct GeoPoseResponse {
    std::string type = "geopose"; //ex. geopose
    std::string id; // UUID
    std::time_t timestamp; // TODO: uint64_t timestamp;? // The number of milliseconds since the Unix Epoch.
    struct GeoPoseAccuracy accuracy;
    struct GeoPose geopose;
};

struct GeoPoseRequest {
    std::string type = "geopose"; //ex. geopose
    std::string id; // UUID
    std::time_t timestamp; // TODO: uint64_t? timestamp;? // The number of milliseconds since the Unix Epoch.
    std::vector<Sensor> sensors;
    SensorReadings sensorReadings;
    std::vector<GeoPoseResponse> priorPoses; // [optional] // previous geoposes // TODO: are these of type GeoPose or GeoPoseResponse?
};

// TODO: add protocol version number in request and response

} // namespace oscp

#endif // _OSCP_GEOPOSE_PROTOCOL_H_
