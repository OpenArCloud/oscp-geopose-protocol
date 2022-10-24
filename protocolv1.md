# oscp-geopose-protocol
OSCP GeoPose Protocol



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
  sequenceNumber: number;
  imageFormat: string; //ex. RGBA32, GRAY8, DEPTH
  size: number[]; //width, height
  imageBytes: string; //base64 encoded data
  imageOrientation?: ImageOrientation; 
}

//aligns with https://w3c.github.io/geolocation-sensor/
export interface GeolocationReading {
  latitude: number;
  longitude: number;
  altitude: number;
  accuracy: number;
  altitudeAccuracy: number;
  heading: number;
  speed: number;
}

export interface WiFiReading {
  BSSID: string;
  frequency: number;
  RSSI: number;
  SSID: string;
  scanTimeStart: number;  // The number of milliseconds* since the Unix Epoch.
  scanTimeEnd: number;  // The number of milliseconds* since the Unix Epoch.
}

export interface BluetoothReading {
  address: string;
  RSSI: number;
  name: string;
}

export interface AccelerometerReading {
  x: number;
  y: number;
  z: number;
}

export interface GyroscopeReading {
  x: number;
  y: number;
  z: number;
}

export interface MagnetometerReading {
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
  params?: CameraParam;
}

export interface SensorReading {
  timestamp: number;  //  The number of milliseconds* since the Unix Epoch.
  sensorId: string;
  privacy: Privacy;
  reading?: (CameraReading | GeolocationReading | WiFiReading | BluetoothReading | AccelerometerReading | GyroscopeReading | MagnetometerReading);
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
  sensorReadings: SensorReading[];
  priorPoses?: GeoPoseResp[]; //previous geoposes
}
```

## GeoPose Protocol Response


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

