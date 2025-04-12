// Open AR Cloud GeoPoseProtocol C++ implementation
// Created based on the protocol definition:
// https://github.com/OpenArCloud/oscp-geopose-protocol

// Created by Gabor Soros, Nokia Bell Labs, 2023
// Copyright 2023 Nokia
// Licensed under the MIT License
// SPDX-License-Identifier: MIT


// #define CPPHTTPLIB_OPENSSL_SUPPORT 1 // TODO: enable SSL here
#include <httplib.h>

#include <oscp-gpp/geoposeprotocol.h>
#include <oscp-gpp/geoposeprotocol_json.h>

bool verify_version_header(const httplib::Headers& headers) {
    if (headers.find("Accept") == headers.end()) {
        throw std::invalid_argument("There is no Accept header in the request");
    }
    for (auto itr = headers.begin(); itr != headers.end(); itr++){
        if ((itr->first).compare("Accept") == 0) {
            std::string acceptHeader = itr -> second;
            std::cout << "Accept header: " << acceptHeader << std::endl;
            if (acceptHeader.find("application/vnd.oscp+json") == std::string::npos) {
                throw std::invalid_argument("The Accept header is expected to contain application/vnd.oscp+json");
            }
            if (acceptHeader.find("version=") == std::string::npos) {
                throw std::invalid_argument("The Accept header is expected to contain version=");
            }
            int versionMajor = 0;
            int versionMinor = 0;
            size_t vStrBegin = acceptHeader.find("version=") + std::string("version=").length();
            size_t vStrEnd = acceptHeader.find(";", vStrBegin);
            std::string vStr = acceptHeader.substr(vStrBegin, vStrEnd);
            if (vStr.find(".") == std::string::npos) {
                versionMajor = std::stoi(vStr); // there is no minor
            }
            std::string vMajorStr = vStr.substr(0, vStr.find("."));
            std::string vMinorStr = vStr.substr(vStr.find(".") + 1, vStr.find(";"));
            versionMajor = std::stoi(vMajorStr);
            versionMinor = std::stoi(vMinorStr);
            std::cout << "Version: " << versionMajor << " " << versionMinor << std::endl;
            if (versionMajor != 2 && versionMinor != 0) {
                throw std::invalid_argument("This server supports only GPP version=2.0");
            }
        }
    }
    return true;
}

int main(int argc, char* argv[])
{
    try {
        std::cout << "Starting GPP Server..." << std::endl;

        if (argc < 2) {
            throw std::invalid_argument("Usage: oscp-gpp-server <CONFIG_PATH>");
        }

        std::string myConfigPath = argv[1];
        std::ifstream myConfigFile(myConfigPath);
        if (!myConfigFile.is_open()) {
            throw std::invalid_argument("Could not open file " + myConfigPath);
        }
        nlohmann::json myConfig = json::parse(myConfigFile);


        httplib::Server server;

        server.Get("/geopose", [](const httplib::Request& req, httplib::Response& res) {
            res.set_content("{\"status\": \"running\"}", "application/json");\
        });

        server.Post("/geopose", [&](const httplib::Request& req, httplib::Response& res) {
            try {
                verify_version_header(req.headers);

                nlohmann::json requestDataJson = json::parse(req.body);
                oscp::GeoPoseRequest gppRequest = requestDataJson.get<oscp::GeoPoseRequest>();

                // DEBUG
                nlohmann::json requestDataJsonNoImage = requestDataJson;
                requestDataJsonNoImage["sensorReadings"]["cameraReadings"][0]["imageBytes"] = "<IMAGE_BASE64>";
                std::cout << "REQUEST JSON:" << std::endl << requestDataJsonNoImage << std::endl;

                // TODO:
                // ...
                // here comes the call to VPS implementation
                // ...
                // right now we just fill in the example values provided in the config file
                oscp::GeoPose myGeoPose;
                myGeoPose.quaternion.x = myConfig["geopose"]["quaternion"]["x"];
                myGeoPose.quaternion.y = myConfig["geopose"]["quaternion"]["y"];
                myGeoPose.quaternion.z = myConfig["geopose"]["quaternion"]["z"];
                myGeoPose.quaternion.w = myConfig["geopose"]["quaternion"]["w"];
                myGeoPose.position.lat = myConfig["geopose"]["position"]["lat"];
                myGeoPose.position.lon = myConfig["geopose"]["position"]["lon"];
                myGeoPose.position.h = myConfig["geopose"]["position"]["h"];

                oscp::GeoPoseResponse gppResponse;
                gppResponse.id = gppRequest.id;
                gppResponse.timestamp = gppRequest.timestamp;
                gppResponse.geopose = myGeoPose;

                nlohmann::json responseDataJson = gppResponse; // automatic conversion
                std::string responseDataString = responseDataJson.dump();

                // DEBUG
                std::cout << "RESPONSE JSON" << std::endl << responseDataJson << std::endl;

                res.set_content(responseDataString, "application/json");
            } catch (std::exception& e) {
                std::string errorMessage = std::string(e.what());
                std::cout << "Exception occurred: " << errorMessage << std::endl;
                res.set_content("{\"error\":\"" + errorMessage + "\"}", "application/json");
                res.status = 400; // bad request
            }
        });

        server.listen("0.0.0.0", 8080);

    } catch (std::exception& e) {
        std::cout << "Exception occurred: " + std::string(e.what()) << std::endl;
        return -1;
    }

    return 0;
}