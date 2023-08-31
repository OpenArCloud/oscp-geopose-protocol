# Open AR Cloud GeoPoseProtocol - Python implementation
# Created by Gabor Soros, 2023
#
# Copyright 2023 Nokia
# Licensed under the MIT License
# SPDX-License-Identifier: MIT


from flask import Flask, request, jsonify, make_response, abort
from argparse import ArgumentParser
import base64
from oscp.geoposeprotocol import *


parser = ArgumentParser()
parser.add_argument(
    '--config', '-config',
    type=str,
    required = True,
    default = None
)
args=parser.parse_args()

with open(args.config, 'r') as f:
    config = json.load(f)
    f.close()
print("Server config:")
print(config)


app = Flask(__name__)

@app.route('/geopose', methods=['GET'])
def status():
    return make_response("{\"status\": \"running\"}", 200)


@app.route('/geopose', methods=['POST'])
def localize():
    jdata = request.get_json()
    geoPoseRequest = GeoPoseRequest.fromJson(jdata)

    if len(geoPoseRequest.sensorReadings.cameraReadings) < 1:
        print('request has no image')
        abort(400, description='request has no camera readings')
    if geoPoseRequest.sensorReadings.cameraReadings[0].imageBytes is None:
        print('request has no image')
        abort(400, description='request has no image')
    imgdata = base64.b64decode(geoPoseRequest.sensorReadings.cameraReadings[0].imageBytes)

    # DEBUG
    #print("Request:")
    #print(geoPoseRequest.toJson())
    #print()


    # TODO:
    # ...
    # here comes the call to VPS implementation
    # ...
    # right now we just fill in the example values provided in the config file
    geoPose = GeoPose()
    geoPose.quaternion.x = config["geopose"]["quaternion"]["x"]
    geoPose.quaternion.y = config["geopose"]["quaternion"]["y"]
    geoPose.quaternion.z = config["geopose"]["quaternion"]["z"]
    geoPose.quaternion.w = config["geopose"]["quaternion"]["w"]
    geoPose.position.lat = config["geopose"]["position"]["lat"]
    geoPose.position.lon = config["geopose"]["position"]["lon"]
    geoPose.position.h = config["geopose"]["position"]["h"]

    geoPoseResponse = GeoPoseResponse(id = geoPoseRequest.id, timestamp = geoPoseRequest.timestamp)
    geoPoseResponse.geopose = geoPose

    # DEBUG
    #print("Response:")
    #print(geoPoseResponse.toJson())
    #print()

    response = make_response(geoPoseResponse.toJson(), 200)
    return response

if __name__ == '__main__':
    app.run(host='0.0.0.0', port=8080, debug=False)
