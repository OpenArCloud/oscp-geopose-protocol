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
  baseline?: number; //dist between lenses on stereo camera
}

export interface Privacy {
  dataRentention: string[]; //acceptable policies for server-side data retention
  dataAcceptableUse: string[]; //acceptable policies for server-side data use
  dataSanitizationApplied: string[]; //client-side data sanitization applied
  dataSanitizationRequested: string[]; //server-side data sanitization requested
}

export interface CameraReading {
  sequenceNumber: number;
  imageFormat: string; //ex. RGBA32, GRAY8, DEPTH
  height: number;
  width: number;
  imageBytes: string; //base64 encoded data
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

export interface Sensor {
  id: string;
  name?: string;
  type: string; //camera, geolocation
  params?: CameraParam
}

export interface SensorReading {
  timestamp: Date;
  sensorId: string;
  privacy: Privacy;
  reading?: (CameraReading | GeolocationReading)
}

export interface GeoPose {
  longitude: number;
  latitude: number;
  ellipsoidHeight: number;
  quaternion: number[];
}

export interface GeoPoseResp {
  id: string;
  timestamp: Date;
  accuracy: number;  
  type: string; //ex. geopose
  pose: GeoPose; 
}

export interface GeoPoseReq {
  id: string;
  timestamp: Date;
  type: string; //ex. geopose
  sensors: Sensor[];
  sensorReadings: SensorReading[];
  priorPose?: GeoPoseResp[]; //previous geoposes
}
```

## GeoPose Protocol Response (version 1)


```js
export interface GeoPose {
  longitude: number;
  latitude: number;
  ellipsoidHeight: number;
  quaternion: number[];
}

export interface GeoPoseResp {
  id: string;
  timestamp: Date;
  accuracy: number;  
  type: string; //ex. geopose
  pose: GeoPose; 
}
```

## Exclusions (version 1)

- streaming
- video
- other sensor types ex. LIDAR
- point cloud
- image features

