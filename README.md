# oscp-geopose-protocol
OSCP GeoPose Protocol

## Purpose

Early version of a standard GeoPose API i.e. a request/response protocol for visual localization. The GeoPose representation will be formalized through the [OGC GeoPose Working Group](https://www.ogc.org/projects/groups/geoposeswg).


## GeoPose Protocol Request (version 1)


```js
export interface CameraIntrinsics {
  height: number;
  width: number;
  focal: number[]; //fx,fy
  center: number[]; //cx, cy
}

export interface CameraExtrinsics {
  rotationMatrix: number[]; //3x3
  translationMatrix: number[]; //3x1
}

export interface CameraDistortion {
  model: string; //Brown or Fisheye
  coefficients: number[]; //[r0 r1 r2 t0 t1] or [r0, r1, r2, r3]
}

export interface CameraOptions {
  minDepth?: number; // for depth image
  maxDepth?: number; // for depth image
  minDisparity?: number; // for disparity image
  maxDisparity?: number; // for disparity image
  baseline?: number; //dist between lenses on stereo camera
}

export interface Camera {
  index: string;
  intrinsics?: CameraIntrinsics;
  extrinsics?: CameraExtrinsics;
  distortion?: CameraDistortion;
  options?: CameraOptions;
}

export interface Privacy {
  dataRentention: string[]; //acceptable policies for server-side data retention
  dataAcceptableUse: string[]; //acceptable policies for server-side data use
  dataSanitizationApplied: string[]; //client-side data sanitization applied
  dataSanitizationRequested: string[]; //server-side data sanitization requested
}

export interface Image {
  cameraIndex: number;
  sequenceIndex: number;
  timestamp: Date;
  imageFormat: string; //ex. RGBA32, GRAY8, DEPTH
  height: number;
  width: number;
  imageBytes: string; //base64 encoded data
  privacy: Privacy;
}

//aligns with https://w3c.github.io/geolocation-sensor/
export interface Geolocation {
  timestamp: Date;
  latitude: number;
  longitude: number;
  altitude: number;
  accuracy: number;
  altitudeAccuracy: number;
  heading: number;
  speed: number;
  privacy: Privacy;
}

export interface SensorReadings {
  image?: Image[];
  geolocation?: Geolocation[];
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
  type: string; //ex. localization-geopose
  camera?: Camera[];
  sensorReadings: SensorReadings;
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

