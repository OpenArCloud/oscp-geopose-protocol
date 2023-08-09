// Open AR Cloud GeoPoseProtocol C++ implementation
// Created based on the protocol definition:
// https://github.com/OpenArCloud/oscp-geopose-protocol

// Created by Gabor Soros, Nokia Bell Labs, 2023
// Copyright 2023 Nokia
// Licensed under the MIT License
// SPDX-License-Identifier: MIT


//#define CPPHTTPLIB_OPENSSL_SUPPORT 1 // TODO: enable SSL here
#include <httplib.h>

#include <uuid.h>
#include <nlohmann/json.hpp>
#include <opencv2/core.hpp>
#include <opencv2/imgcodecs.hpp>

#include <oscp-gpp/geoposeprotocol.h>
#include <oscp-gpp/geoposeprotocol_json.h>
#include <oscp-gpp/base64.h>

#include <nlohmann/json.hpp>

#include <iostream>
#include <string>
#include <vector>

int main(int argc, char* argv[]) {
    try {
        std::cout << "Starting GPP Client..." << std::endl;

        if (argc < 4) {
            throw std::invalid_argument("Usage: oscp-gpp-client <IMAGE_PATH> <CAMERA_PARAMS_PATH> <GEOLOCATION_PARAMS_PATH>");
        }

        std::string myImagePath = argv[1];
        std::cout << "Image path: " << myImagePath << std::endl;
        std::string myCameraParamsPath = argv[2];

        std::cout << "Camera params path: " << myCameraParamsPath << std::endl;
        std::ifstream myCameraParamsFile(myCameraParamsPath);
        if (!myCameraParamsFile.is_open()) {
            throw std::invalid_argument("Could not open file " + myCameraParamsPath);
        }
        nlohmann::json myCameraParamsJson = json::parse(myCameraParamsFile);
        std::string myGeolocationParamsPath = argv[3];
        std::cout << "Geolocation params path: " << myGeolocationParamsPath << std::endl;
        std::ifstream myGeolocationParamsFile(myGeolocationParamsPath);
        if (!myGeolocationParamsFile.is_open()) {
            throw std::invalid_argument("Could not open file " + myGeolocationParamsPath);
        }
        nlohmann::json myGeolocationParamsJson = json::parse(myGeolocationParamsFile);

        oscp::CameraModel myCameraModel = oscp::cameraModelFromString(myCameraParamsJson["camera_model"]);
        // NOTE: camera params: fx, fy, cx, cy, k1, k2, p1, p2
        // Colmap and therefore also GPP ignores the 5th OpenCV coefficient
        std::vector<float> myCameraModelParams = myCameraParamsJson["camera_params"];
        std::string myCameraSensorId = myCameraParamsJson["camera_id"];

        uint64_t myTimestamp =  std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::steady_clock::now().time_since_epoch()).count();
        unsigned int myRequestCounter = 0;

        // Load image
        cv::Mat img = cv::imread(myImagePath, cv::IMREAD_COLOR);
        if (img.empty()) {
            throw std::invalid_argument("Could not open file " + myImagePath);
        }
        // Compress to JPEG (and copy)
        std::vector<uchar> imgJPEG;
        if (!cv::imencode(".jpg", img, imgJPEG)) {
            std::string errorMsg = "WARNING: could not compress image for localization";
            std::cout << errorMsg << std::endl;
            return -1;
        }
        // Encode JPEG to Base64
        std::string imgBase64 = oscp::base64_encode(imgJPEG.data(), imgJPEG.size());

        // Assemble request
        oscp::GeoPoseRequest geoPoseRequest;
        std::mt19937 rnd{std::random_device()()};
        uuids::uuid_random_generator gen{rnd};
        const uuids::uuid uuid = gen();
        geoPoseRequest.id = uuids::to_string(uuid);
        geoPoseRequest.timestamp = myTimestamp;

        oscp::Sensor cameraSensor;
        cameraSensor.id = myCameraSensorId;
        cameraSensor.type = oscp::SensorType::CAMERA;
        cameraSensor.name = "test_client"; // test
        cameraSensor.model = toString(myCameraModel);
        geoPoseRequest.sensors.push_back(cameraSensor);

        oscp::CameraReading cameraReading;
        cameraReading.imageBytes = imgBase64;
        cameraReading.imageFormat = oscp::ImageFormat::JPG;
        cameraReading.imageOrientation = oscp::ImageOrientation(false, 0.0);
        cameraReading.sequenceNumber = myRequestCounter;
        cameraReading.size[0] = img.cols;
        cameraReading.size[1] = img.rows;
        cameraReading.sensorId = cameraSensor.id;
        cameraReading.timestamp = myTimestamp;
        oscp::CameraParameters cameraParameters;
        cameraParameters.model = myCameraModel;
        cameraParameters.modelParams = myCameraModelParams;
        cameraReading.params = cameraParameters;
        geoPoseRequest.sensorReadings.cameraReadings.push_back(cameraReading);

        oscp::GeolocationReading geolocationReading;
        geolocationReading.latitude = myGeolocationParamsJson["lat"];
        geolocationReading.longitude = myGeolocationParamsJson["lon"];
        geolocationReading.altitude = myGeolocationParamsJson["h"];
        geoPoseRequest.sensorReadings.geolocationReadings.push_back(geolocationReading);

        nlohmann::json requestDataJson = geoPoseRequest; // automatic conversion
        std::string requestDataString = requestDataJson.dump();

        // DEBUG
        nlohmann::json requestDataJsonNoImage = requestDataJson;
        requestDataJsonNoImage["sensorReadings"]["cameraReadings"][0]["imageBytes"] = "<IMAGE_BASE64>";
        std::cout << "REQUEST JSON:" << std::endl << requestDataJsonNoImage << std::endl;

        httplib::Client client("localhost", 8080);

        if (auto res = client.Post("/geopose", requestDataString, "application/json")) {
            if (res->status == 200) {
                std::string responseDataString = res->body;
                json responseDataJson = json::parse(responseDataString);

                // DEBUG
                std::cout << "RESPONSE JSON" << std::endl << responseDataJson << std::endl;

                oscp::GeoPoseResponse geoPoseResponse = responseDataJson.get<oscp::GeoPoseResponse>();
                oscp::GeoPose geoPose = geoPoseResponse.geopose;

                std::cout << "Successful VPS localization!" << std::endl << std::setprecision(10)
                                    << "  " << geoPose.quaternion.x
                                    << ", " << geoPose.quaternion.y
                                    << ", " << geoPose.quaternion.z
                                    << ", " << geoPose.quaternion.w << std::endl
                                    << "  " << geoPose.position.lat
                                    << ", " << geoPose.position.lon
                                    << ", " << geoPose.position.h << std::endl;
            } else {
                std::cout << "HTTP status: " << httplib::detail::status_message(res->status) << std::endl;
                std::cout << res->body << std::endl;
            }
        } else {
            auto err = res.error();
            std::cout << "HTTP error: " << httplib::to_string(err) << std::endl;
        }
    } catch (std::exception& e) {
        std::cout << "Exception occurred: " + std::string(e.what()) << std::endl;
        return -1;
    }

    return 0;
}
