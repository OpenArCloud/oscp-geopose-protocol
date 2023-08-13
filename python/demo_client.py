# Open AR Cloud GeoPoseProtocol - Python implementation
# Created by Gabor Soros, 2023
#
# Copyright 2023 Nokia
# Licensed under the MIT License
# SPDX-License-Identifier: MIT
#
# Created based on the protocol definition:
# https://github.com/OpenArCloud/oscp-geopose-protocol
# and the JavaScript implementation:
# https://github.com/OpenArCloud/gpp-access/


from PIL import Image
from oscp.geoposeprotocol import *
import time
import json
import base64
import requests
from argparse import ArgumentParser
from datetime import datetime, timezone


###print(json.dumps(GeoPoseRequest(), default=lambda o: o.__dict__))


parser = ArgumentParser()
parser.add_argument(
    '--url', '-url',
    type=str,
    default='http://127.0.0.1:8080/geopose'
)
parser.add_argument(
    '--image', '-image',
    type=str,
    required = True,
    default = None
)
parser.add_argument(
    '--camera', '-camera',
    type=str,
    required = True,
    default = None
)
parser.add_argument(
    '--geolocation', '-geolocation',
    type=str,
    required = True,
    default = None
)
args=parser.parse_args()


with open(args.image, 'rb') as f:
    image = f.read()
    image_base64 = base64.b64encode(image).decode('utf-8')
    f.close()

# open it again with PIL just to find out its size
image = Image.open(args.image)

with open(args.camera, 'r') as f:
    camera_config = json.load(f)
    f.close()
print("Camera config:")
print(camera_config)

with open(args.geolocation, 'r') as f:
    geolocation_config = json.load(f)
    f.close()
print("(Coarse) geolocation config:")
print(geolocation_config)


kCameraSensorId = "my_camera_sensor"
cameraReading = CameraReading(sensorId=kCameraSensorId)
cameraReading.timestamp = datetime.now(timezone.utc).timestamp()*1000 # milliseconds since epoch
cameraReading.sensorId = kCameraSensorId
cameraReading.imageFormat = ImageFormat.RGBA32
cameraReading.size = [image.width, image.height]
cameraReading.imageBytes = image_base64
cameraReading.sequenceNumber = 1
cameraReading.imageOrientation = ImageOrientation()
cameraReading.params = CameraParameters(model=camera_config["camera_model"], modelParams=camera_config["camera_params"])

kGeolocationSensorId = "my_gps_sensor"
geolocationReading = GeolocationReading(sensorId=kGeolocationSensorId)
geolocationReading.timestamp = datetime.now(timezone.utc).timestamp()*1000 # milliseconds since epoch
geolocationReading.latitude = geolocation_config["lat"]
geolocationReading.longitude = geolocation_config["lon"]
geolocationReading.altitude = geolocation_config["h"]

geoPoseRequest = GeoPoseRequest()
geoPoseRequest.timestamp = datetime.now(timezone.utc).timestamp()*1000 # milliseconds since epoch
geoPoseRequest.sensors.append(Sensor(type = SensorType.CAMERA, id=kCameraSensorId))
geoPoseRequest.sensorReadings.cameraReadings.append(cameraReading)
geoPoseRequest.sensors.append(Sensor(type = SensorType.GEOLOCATION, id=kGeolocationSensorId))
geoPoseRequest.sensorReadings.geolocationReadings.append(geolocationReading)

try:
    headers = {"Content-Type":"application/json"}
    body = geoPoseRequest.toJson()

    # DEBUG
    geoPoseRequest.sensorReadings.cameraReadings[0].imageBytes = "<IMAGE_BASE64>"
    print("Request (without image):")
    print(geoPoseRequest.toJson())
    print()

    response = requests.post(args.url, headers=headers, data=body)
    print(f'Status: {response.status_code}')
    jdata = response.json()
    geoPoseResponse = GeoPoseResponse.fromJson(jdata)

    # DEBUG:
    print("Response:")
    print(geoPoseResponse.toJson())
    print()

except Exception as e:
    print(f'err: {e}')
