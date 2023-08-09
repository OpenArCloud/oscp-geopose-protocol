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