# oscp-geopose-protocol
OSCP GeoPose Protocol

## Purpose

Early version of a standard GeoPose API i.e. a request/response protocol for visual localization. The GeoPose representation will be formalized through the [OGC GeoPose Working Group](https://www.ogc.org/projects/groups/geoposeswg).


## GeoPose Protocol Request (version 1)


```js
export interface CameraParam {
  model?: string; //UNKNOWN_CAMERA, SIMPLE_PINHOLE, SIMPLE_RADIAL, RADIAL, PINHOLE, OPENCV, FULL_OPENCV
  modelParams?: number[];
  minMaxDepth?: number[]; // for depth image
  minMaxDisparity?: number[]; // for disparity image
}

export interface Privacy {
  dataRetention: string[]; //acceptable policies for server-side data retention
  dataAcceptableUse: string[]; //acceptable policies for server-side data use
  dataSanitizationApplied: string[]; //client-side data sanitization applied
  dataSanitizationRequested: string[]; //server-side data sanitization requested
}

export interface ImageOrientation {
  mirrored: boolean;  // Value as provided from Camera sensor
  rotation: number;  // Value as provided from Camera sensor
}

export interface CameraReading {
  timestamp: number;  //  The number of milliseconds* since the Unix Epoch.
  sensorId: string;
  privacy: Privacy; 
  sequenceNumber: number;
  imageFormat: string; //ex. RGBA32, GRAY8, DEPTH
  size: number[]; //width, height
  imageBytes: string; //base64 encoded data
  imageOrientation?: ImageOrientation; 
  params?: CameraParam;
}

//aligns with https://w3c.github.io/geolocation-sensor/
export interface GeolocationReading {
  timestamp: number;  //  The number of milliseconds* since the Unix Epoch.
  sensorId: string;
  privacy: Privacy; 
  latitude: number;
  longitude: number;
  altitude: number;
  accuracy: number;
  altitudeAccuracy: number;
  heading: number;
  speed: number;
}

export interface WiFiReading {
  timestamp: number;  //  The number of milliseconds* since the Unix Epoch.
  sensorId: string;
  privacy: Privacy; 
  BSSID: string;
  frequency: number;
  RSSI: number;
  SSID: string;
  scanTimeStart: number;  // The number of milliseconds* since the Unix Epoch.
  scanTimeEnd: number;  // The number of milliseconds* since the Unix Epoch.
}

export interface BluetoothReading {
  timestamp: number;  //  The number of milliseconds* since the Unix Epoch.
  sensorId: string;
  privacy: Privacy; 
  address: string;
  RSSI: number;
  name: string;
}

export interface AccelerometerReading {
  timestamp: number;  //  The number of milliseconds* since the Unix Epoch.
  sensorId: string;
  privacy: Privacy; 
  x: number;
  y: number;
  z: number;
}

export interface GyroscopeReading {
  timestamp: number;  //  The number of milliseconds* since the Unix Epoch.
  sensorId: string;
  privacy: Privacy; 
  x: number;
  y: number;
  z: number;
}

export interface MagnetometerReading {
  timestamp: number;  //  The number of milliseconds* since the Unix Epoch.
  sensorId: string;
  privacy: Privacy; 
  x: number;
  y: number;
  z: number;
}

export interface Position {
  lon: number;
  lat: number;
  h: number;
}

export interface Quaternion {
  x: number;
  y: number;
  z: number;
  w: number;
}

export interface Vector3 {
  x: number;
  y: number;
  z: number;
}

export interface Sensor {
  id: string;
  name?: string;
  model?: string;
  rigIdentifier?: string
  rigRotation?: Quaternion; //rotation quaternion from rig to sensor
  rigTranslation?: Vector3; //translation vector from rig to sensor
  type: string; //camera, geolocation, wifi, bluetooth, accelerometer, gyroscope, magnetometer
}

export interface SensorReadings {
  cameraReadings?: CameraReading[];
  geolocationReadings?: GeolocationReading[];
  wiFiReadings?: WiFiReading[];
  bluetoothReadings?: BluetoothReading[];
  accelerometerReadings?: AccelerometerReading[];
  gyroscopeReadings?: GyroscopeReading[];
  magnetometerReadings?: MagnetometerReading[];
}

export interface GeoPoseAccuracy {
  position: number  //  mean for all components in meters
  orientation: number // mean for all 3 angles in degrees
}

export interface GeoPose {
  position: Position;
  quaternion: Quaternion;
}

export interface GeoPoseResp {
  id: string;
  timestamp: number;  //  The number of milliseconds* since the Unix Epoch.
  accuracy: GeoPoseAccuracy;
  type: string; //ex. geopose
  geopose: GeoPose; 
}

export interface GeoPoseReq {
  id: string;
  timestamp: number;  //  The number of milliseconds* since the Unix Epoch.
  type: string; //ex. geopose
  sensors: Sensor[];
  sensorReadings: SensorReadings;
  priorPoses?: GeoPoseResp[]; //previous geoposes
}
```

## GeoPose Protocol Response (version 1)


```js
export interface Position {
  lon: number;
  lat: number;
  h: number;
}

export interface Quaternion {
  x: number;
  y: number;
  z: number;
  w: number;
}

export interface GeoPoseAccuracy {
  position: number  //  mean for all components in meters
  orientation: number // mean for all 3 angles in degrees
}

export interface GeoPose {
  position: Position;
  quaternion: Quaternion;
}

export interface GeoPoseResp {
  id: string;
  timestamp: number;  //  The number of milliseconds* since the Unix Epoch.
  accuracy: GeoPoseAccuracy;
  type: string; //ex. geopose
  geopose: GeoPose; 
}
```

## Exclusions (version 1)

- streaming
- video
- other sensor types ex. LIDAR
- point cloud
- image features

