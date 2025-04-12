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
import re

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

def parse_accept_type(accept_header):
    TYPE_REGEX = re.compile(
        r'application/vnd\.oscp\+json'# OSCP JSON
    )
    type = TYPE_REGEX.search(accept_header)
    if type == None:
        return False
    return True

def parse_accept_version(accept_header):
    VERSION_REGEX = re.compile(
        r'version='                 # version=
        r'(?P<major>[0-9]+)'        # capture major number
        r'(?:.(?P<minor>[0-9]+))?'  # capture minor number if exists
    )
    version = VERSION_REGEX.search(accept_header)
    if version == None:
        return False, None, None
    #print(version.groupdict())
    majorStr = version.groupdict()['major']
    minorStr = version.groupdict()['minor']
    try:
        major = int(majorStr)
        minor = None if minorStr == None else int(minorStr)
    except:
        return False, None, None
    return True, major, minor

def verify_version_header(headers):
    if not 'Accept' in headers.keys():
        return False, None, None
    if not parse_accept_type(headers.get('Accept')):
        return False, None, None
    return parse_accept_version(headers.get('Accept'))

@app.route('/geopose', methods=['POST'])
def localize():
    success, versionMajor, versionMinor = verify_version_header(request.headers)
    if not success:
        print('request has no or malformed Accept header. Add the header application/vnd.oscp+json;version=2.0')
        abort(400, description='request has no or malformed Accept header. Add the header application/vnd.oscp+json;version=2.0')
    print(f"Version: {versionMajor} {versionMinor}")
    if versionMajor != 2 or versionMinor != 0:
        print('This server supports only GPP v2.0')
        abort(400, description='This server supports only GPP v2.0')

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
    #geoPoseRequest.sensorReadings.cameraReadings[0].imageBytes = "<IMAGE_BASE64>"
    #print("Request (without image):")
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
