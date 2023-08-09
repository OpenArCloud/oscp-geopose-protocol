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

from datetime import datetime
from enum import Enum
import uuid
import json
from geopose import *
import sys

'''
Sensor types usable with the GeoPose protocol
Use when creating a new Sensor object.
'''
class SensorType(str, Enum):
    CAMERA = 'camera'
    GEOLOCATION = 'geolocation'
    WIFI = 'wifi'
    BLUETOOTH = 'bluetooth'
    ACCELEROMETER = 'accelerometer'
    GYROSCOPE = 'gyroscope'
    MAGNETOMETER = 'magnetometer'
    UNKNOWN = 'unknown'
    # TODO: add altitude sensor

'''
Image formats usable with the CameraReading object
Use when creating a SensorReading object for a new Sensor of type 'camera'.
'''
class ImageFormat(str, Enum):
    RGBA32 = 'RGBA32'
    GRAY8 = 'GRAY8'
    DEPTH = 'DEPTH'
    JPG = 'JPG'
    UNKNOWN = 'unknown'

class ImageOrientation(object):
    def __init__(self, mirrored = False, rotation = 0.0):
        self.mirrored = mirrored
        self.rotation = rotation

    def __str__(self):
        return "{" + \
            "mirrored:" + str(self.mirrored) + "," + \
            "rotation:" + str(self.rotation) + \
        "}"

# The camera models of Colmap are used here
# See https://colmap.github.io/cameras.html
class CameraModel(str, Enum):
    SIMPLE_PINHOLE = 'SIMPLE_PINHOLE' # f, cx, cy
    PINHOLE = 'PINHOLE' # fx, fy, cx, cy
    SIMPLE_RADIAL = 'SIMPLE_RADIAL' # f, cx, cy, k
    RADIAL = 'RADIAL' # f, cx, cy, k1, k2
    OPENCV = 'OPENCV' # fx, fy, cx, cy, k1, k2, p1, p2
    OPENCV_FISHEYE = 'OPENCV_FISHEYE' # fx, fy, cx, cy, k1, k2, k3, k4
    FULL_OPENCV = 'FULL_OPENCV' # fx, fy, cx, cy, k1, k2, p1, p2, k3, k4, k5, k6
    FOV = 'FOV' # fx, fy, cx, cy, omega
    SIMPLE_RADIAL_FISHEYE = 'SIMPLE_RADIAL_FISHEYE' # f, cx, cy, k
    RADIAL_FISHEYE = 'RADIAL_FISHEYE' # f, cx, cy, k1, k2
    THIN_PRISM_FISHEYE = 'THIN_PRISM_FISHEYE' # fx, fy, cx, cy, k1, k2, p1, p2, k3, k4, sx1, sy1
    UNKNOWN = 'UNKNOWN'

class CameraParameters(object):
    def __init__(self, model = CameraModel.UNKNOWN, modelParams = [], minMaxDepth = [], minMaxDisparity = []):
        self.model = model # [optional] // TODO: string in the v1 standard, but enum is better suited here
        self.modelParams = modelParams # [optional]
        self.minMaxDepth = minMaxDepth # [optional] // for depth image
        self.minMaxDisparity = minMaxDisparity # [optional] // for disparity image

    def __str__(self):
        return "{" + \
            "model:" + str(self.model) + "," + \
            "modelParams:" + str(self.modelParams) + "," + \
            "minMaxDepth:" + str(self.minMaxDepth) + "," + \
            "minMaxDisparity:" + str(self.minMaxDisparity) + \
        "}"

class Privacy(object):
    def __init__(self, dataRetention = [], dataAcceptableUse = [], dataSanitizationApplied = [], dataSanitizationRequested = []):
        self.dataRetention = dataRetention # acceptable policies for server-side data retention
        self.dataAcceptableUse = dataAcceptableUse # acceptable policies for server-side data use
        self.dataSanitizationApplied = dataSanitizationApplied # client-side data sanitization applied
        self.dataSanitizationRequested = dataSanitizationRequested # server-side data sanitization requested

    def __str__(self):
        return "{" + \
            "dataRetention:" + str(self.dataRetention) + "," + \
            "dataAcceptableUse:" + str(self.dataAcceptableUse) + "," + \
            "dataSanitizationApplied:" + str(self.dataSanitizationApplied) + "," + \
            "dataSanitizationRequested:" + str(self.dataSanitizationRequested) + \
        "}"

class CameraReading(object):
    def __init__(self, timestamp = 0, sensorId = "", privacy = Privacy(),
                sequenceNumber = 0, imageFormat = ImageFormat.UNKNOWN, size = [0,0], imageBytes = [],
                imageOrientation = ImageOrientation(), params = CameraParameters()):
        self.timestamp = timestamp # The number of milliseconds* since the Unix Epoch.
        self.sensorId = sensorId
        self.privacy = privacy
        self.sequenceNumber = sequenceNumber
        self.imageFormat = imageFormat # TODO: string or enum?
        self.size = size# # width, height
        self.imageBytes = imageBytes # base64 encoded image data
        self.imageOrientation = imageOrientation # [optional]
        self.params = params

    def __str__(self):
        return "{" + \
            "timestamp:" + str(self.timestamp) + "," + \
            "sensorId:" + str(self.sensorId) + "," + \
            "privacy:" + str(self.privacy) + "," + \
            "sequenceNumber:" + str(self.sequenceNumber) + "," + \
            "imageFormat:" + str(self.imageFormat) + "," + \
            "size:" + str(self.size) + "," + \
            "imageBytes:" + str(self.imageBytes) + "," + \
            "imageOrientation:" + str(self.imageOrientation) + "," + \
            "params:" + str(self.params) + \
        "}"

class GeolocationReading(object):
    # aligns with https://w3c.github.io/geolocation-sensor/
    def __init__(self, timestamp = 0, sensorId = "", privacy = Privacy(),
                 latitude = 0.0, longitude = 0.0, altitude = 0.0, accuracy = 0.0, altitudeAccuracy = 0.0, heading = 0.0, speed = 0.0):
        self.timestamp = timestamp # The number of milliseconds* since the Unix Epoch.
        self.sensorId = sensorId
        self.privacy = privacy
        self.latitude = latitude
        self.longitude = longitude
        self.altitude = altitude
        self.accuracy = accuracy
        self.altitudeAccuracy = altitudeAccuracy
        self.heading = heading
        self.speed = speed

class WiFiReading(object):
    def __init__(self, timestamp = 0, sensorId = "", privacy = Privacy(),
                 BSSID = "", frequency = 0.0, RSSI = 0.0, SSID = "", scanTimeStart = 0, scanTimeEnd = 0):
        self.timestamp = timestamp # The number of milliseconds* since the Unix Epoch.
        self.sensorId = sensorId
        self.privacy = privacy
        self.BSSID = BSSID
        self.frequency = frequency # TODO: shouldn't this be a frequency range?
        self.RSSI = RSSI # TODO: shouldn't this be a vector?
        self.SSID = SSID
        self.scanTimeStart = scanTimeStart # The number of milliseconds since the Unix Epoch.
        self.scanTimeEnd = scanTimeEnd # The number of milliseconds since the Unix Epoch.

    def __str__(self):
        return "{" + \
            "timestamp:" + str(self.timestamp) + "," + \
            "sensorId:" + str(self.sensorId) + "," + \
            "privacy:" + str(self.privacy) + "," + \
            "BSSID:" + str(self.BSSID) + "," + \
            "frequency:" + str(self.frequency) + ',' + \
            "RSSI:" + str(self.RSSI) + "," + \
            "SSID:" + str(self.SSID) + ',' + \
            "scanTimeStart:" + str(self.scanTimeStart) + "," + \
            "scanTimeEnd:" + str(self.scanTimeEnd) + \
        "}"

class BluetoothReading(object):
    def __init__(self, timestamp = 0, sensorId = "", privacy = Privacy(),
                 address = "", RSSI = 0.0, name = ""):
        self.timestamp = timestamp # The number of milliseconds* since the Unix Epoch.
        self.sensorId = sensorId
        self.privacy = privacy
        self.address = address
        self.RSSI = RSSI # TODO: shouldn't this be a vector?
        self.name = name

    def __str__(self):
        return "{" + \
            "timestamp:" + str(self.timestamp) + "," + \
            "sensorId:" + str(self.sensorId) + "," + \
            "privacy:" + str(self.privacy) + "," + \
            "address:" + str(self.address) + "," + \
            "RSSI:" + str(self.RSSI) + ',' + \
            "name:" + str(self.name) + \
        "}"

class AccelerometerReading(object):
    def __init__(self, timestamp = 0, sensorId = "", privacy = Privacy(),
                 x = 0.0, y = 0.0, z = 0.0):
        self.timestamp = timestamp # The number of milliseconds* since the Unix Epoch.
        self.sensorId = sensorId
        self.privacy = privacy
        self.x = x
        self.y = y
        self.z = z

    def __str__(self):
        return "{" + \
            "timestamp:" + str(self.timestamp) + "," + \
            "sensorId:" + str(self.sensorId) + "," + \
            "privacy:" + str(self.privacy) + "," + \
            "x:" + str(self.x) + "," + \
            "y:" + str(self.y) + ',' + \
            "z:" + str(self.z) + \
        "}"

class GyroscopeReading(object):
    def __init__(self, timestamp = 0, sensorId = "", privacy = Privacy(),
                 x = 0.0, y = 0.0, z = 0.0):
        self.timestamp = timestamp # The number of milliseconds* since the Unix Epoch.
        self.sensorId = sensorId
        self.privacy = privacy
        self.x = x
        self.y = y
        self.z = z

    def __str__(self):
        return "{" + \
            "timestamp:" + str(self.timestamp) + "," + \
            "sensorId:" + str(self.sensorId) + "," + \
            "privacy:" + str(self.privacy) + "," + \
            "x:" + str(self.x) + "," + \
            "y:" + str(self.y) + ',' + \
            "z:" + str(self.z) + \
        "}"

class MagnetometerReading(object):
    def __init__(self, timestamp = 0, sensorId = "", privacy = Privacy(),
                 x = 0.0, y = 0.0, z = 0.0):
        self.timestamp = timestamp # The number of milliseconds* since the Unix Epoch.
        self.sensorId = sensorId
        self.privacy = privacy
        self.x = x
        self.y = y
        self.z = z

    def __str__(self):
        return "{" + \
            "timestamp:" + str(self.timestamp) + "," + \
            "sensorId:" + str(self.sensorId) + "," + \
            "privacy:" + str(self.privacy) + "," + \
            "x:" + str(self.x) + "," + \
            "y:" + str(self.y) + ',' + \
            "z:" + str(self.z) + \
        "}"

class Sensor(object):
    def __init__(self, type = SensorType.UNKNOWN, id = "", name = "", model = "",
                 rigIdentifier = "", rigRotation = Quaternion(), rigTranslation = Vector3()):
        self.type = type # camera, geolocation, wifi, bluetooth, accelerometer, gyroscope, magnetometer
        self.id = id
        self.name = name # [optional]
        self.model = model # [optional] // TODO: is this CameraModel or other model? If CameraModel, it is redundant here and should be inside params only.
        self.rigIdentifier = rigIdentifier # [optional]
        self.rigRotation = rigRotation # [optional] // rotation quaternion from rig to sensor
        self.rigTranslation = rigTranslation # [optional] //  translation vector from rig to sensor

    def __str__(self):
        return "{" + \
            "type:" + str(self.type) + "," + \
            "id:" + str(self.id) + "," + \
            "name:" + str(self.name) + "," + \
            "model:" + str(self.model) + "," + \
            "rigIdentifier:" + str(self.rigIdentifier) + ',' + \
            "rigRotation:" + str(self.rigRotation) + ',' + \
            "rigTranslation:" + str(self.rigTranslation) + \
        "}"


class SensorReadings(object):
    def __init__(self, cameraReadings = [], geolocationReadings = [], accelerometerReading = [], gyroscopeReadings = [],
                    magnetometerReadings = [], wifiReadings = [], bluetoothReadings = []):
        self.cameraReadings = cameraReadings
        self.geolocationReadings = geolocationReadings
        self.accelerometerReading = accelerometerReading
        self.gyroscopeReadings = gyroscopeReadings
        self.magnetometerReadings = magnetometerReadings
        self.wifiReadings = wifiReadings
        self.bluetoothReadings = bluetoothReadings

    def __str__(self):
        return "{" + \
            "cameraReadings:" + str(self.cameraReadings) + "," + \
            "geolocationReadings:" + str(self.geolocationReadings) + "," + \
            "accelerometerReading:" + str(self.accelerometerReading) + "," + \
            "gyroscopeReadings:" + str(self.gyroscopeReadings) + "," + \
            "magnetometerReadings:" + str(self.magnetometerReadings) + ',' + \
            "wifiReadings:" + str(self.wifiReadings) + ',' + \
            "bluetoothReadings:" + str(self.bluetoothReadings) + \
        "}"

class GeoPoseAccuracy(object):
    def __init__(self, position = sys.float_info.max, orientation = sys.float_info.max):
        self.position = position # mean for all components in meters
        self.orientation = orientation # mean for all 3 angles in degrees

    def __str__(self):
        return "{" + \
            "y:" + str(self.y) + ',' + \
            "z:" + str(self.z) + \
        "}"

# TODO: add protocol version number in request and response
class GeoPoseResponse(object):
    def __init__(self, type = "geopose", id = str(uuid.uuid4()), timestamp = str(datetime.now(datetime.timezone.utc)),
                accuracy = GeoPoseAccuracy(), geopose = GeoPose()):
        self.type = type # ex. geopose
        self.id = id
        self.timestamp = timestamp # The number of milliseconds since the Unix Epoch.
        self.accuracy = accuracy
        self.geopose = geopose

    def __str__(self):
        return "{" + \
            "type:" + str(self.type) + ',' + \
            "id:" + str(self.id) + ',' + \
            "timestamp:" + str(self.timestamp) + ',' + \
            "accuracy:" + str(self.accuracy) + ',' + \
            "geopose:" + str(self.geopose) + \
        "}"

class GeoPoseRequest(object):
    def __init__(self, type = "geopose", id = str(uuid.uuid4()), timestamp = str(datetime.now(datetime.timezone.utc)),
                 sensors = [], sensorReadings = SensorReadings(), priorPoses = []):
        self.type = type # ex. geopose
        self.id = id
        self.timestamp = timestamp # The number of milliseconds since the Unix Epoch.
        self.sensors = sensors
        self.sensorReadings = sensorReadings
        self.priorPoses = priorPoses # TODO: are these of type GeoPose or GeoPoseResponse?

    def __str__(self):
        return "{" + \
            "type:" + str(self.type) + ',' + \
            "id:" + str(self.id) + ',' + \
            "timestamp:" + str(self.timestamp) + ',' + \
            "sensors:" + str(self.sensors) + ',' + \
            "sensorReadings:" + str(self.sensorReadings) + ',' + \
            "priorPoses:" + str(self.priorPoses) + \
        "}"
