# Geopose and objects pose endpoints
Endpoints are available at https://developer.augmented.city/
### Usage
A geo-pose represents the object's coordinate system position in the world coordinate system, so that it is accurately positioned on the globe, correctly oriented with respect to cardinal directions and vertical axis.
There can be many options with different world coordinate system: ECEF or Local Tangent Plane systems and others (https://en.wikipedia.org/wiki/Local_tangent_plane_coordinates)
And also a number of object’s representations.
Geo-pose consists of rotation part (quaternion) and position part.
By applying these transformations to the points of the object model (rotation first, then translation to position), one can calculate their location in the world.

__Description of geopose and scrs fields__

Geopose called `ecefPose` is intended to place objects in ECEF coordinate system which for example can be directly applied to Cesium platform.
The object's frame should be oriented with +X forward direction and +Z in upwards prior to applying geopose transformations. If it is not, consider to combine the geopose with pre-rotations (using quaternion multiplication routines)
(This is specific to Augmented City)

Geopose called `localPose` is a Local Tangent Plane transformation, which means it places the object in familiar cartesian world with axes aligned to cardinal directions. But it is implied that the object’s origin is located exactly in coordinates "latitude", "longitude", "altitude".
This realization can be used by AR-enabled tools. It deals with object and world coordinate systems specific to current ARKit/ARCore. Object’s forward direction towards -Z and Up direction is +Y. World’s X points east, Y up, Z south
(This is specific to Augmented City)

Geopose called `geopose` is specified by OGC (https://www.ogc.org/standard/geopose/). It holds position as latitude, longitude, and height above the WGS84 ellopsoid, as well as a rotation quaternion w.r.t. the local East-North-Up coordinate system.

#### /geopose - get geopose of an image

__Response example__
```json
{
    "id": "9089876676575754",
    "geopose": {
        "position": {
            "lat": 59.93522601509476,
            "lon": 30.27066840280691,
            "h": -0.08433365951111288
        },
        "quaternion": {
            "w": -0.007102287767713538,
            "x": -0.031042676130458968,
            "y": -0.023274419381073084,
            "z": -0.9992218027893498
        }
    },
    "accuracy": {
        "position": -1,
        "orientation": -1
    },
    "timestamp": "Fri, 04 Dec 2020 19:41:10 GMT",
    "type": "geopose"
}
```


#### /scrs/geopose_objs_local - get geopose of an image and simultaneously retrieve all objects in the neighborhood

See https://developer.augmented.city/doc/v2#operation/getGeoposeObjsLocal

```
Camera's GeoPose:
geopose.geopose.position.lat - geodetic coordinates
geopose.geopose.position.lon - geodetic coordinates
geopose.geopose.position.h - camera’s height. The WGS84 ellipsoid level is treated as zero height
geopose.geopose.quaternion.x|y|z|w - rotation of the camera w.r.t ENU

geopose.ecefPose.orientation.x|y|z|w - rotation transformation (ECEF coordinate system)
geopose.ecefPose.position.x|y|z - The object’s origin coordinates relative to Earth’s mass center (ECEF coordinate system), units - meters.

Object's GeoPose:
scrs[0].content.geopose.position.h - object’s origin height about ground level
scrs[0].content.geopose.position.lat - geodetic coordinates
scrs[0].content.geopose.position.lon - geodetic coordinates
scrs[0].content.geopose.quaternion.x|y|z|w - rotation transformation(object coordinate system to ARKit/ARCore coordinate system)

scrs[0].content.geopose.ecefPose.quaternion.x|y|z|w - rotation transformation (ECEF coordinate system)
scrs[0].content.geopose.ecefPose.position.x|y|z - The object’s origin coordinates relative to Earth’s mass center (ECEF coordinate system), units - meters.

```
__Response example__
```json
{
    "geopose": {
        "id": "9089876676575754",
        "timestamp": "Thu, 10 Dec 2020 17:14:03 GMT",
        "type": "geopose",
        "geopose": {
            "position": {
                "lat": 59.93522609622502,
                "lon": 30.27066834299855,
                "h": -0.08380349220203223,
            },
            "quaternion": {
                "w": -0.007090709830811782,
                "x": -0.031015972010745515,
                "y": -0.02327018097625009,
                "z": -0.9992228129860009
            }
        },
        "accuracy": {
            "position": -1,
            "orientation": -1
        },
        "ecefPose": {
            "position": {
                "x": 2766587.0855982956,
                "y": 1614763.59540361,
                "z": 5496865.242906623
            },
            "orientation": {
                "w": 0.2661153288127459,
                "x": 0.040328282280189406,
                "y": 0.9231699371248662,
                "z": -0.2744331040674282
            }
        }
    },
    "scrs": [
        {
            "id": "8200",
            "tenant": "AugmentedCity",
            "timestamp": "2020-12-10T17:14:01.795301",
            "type": "scr",
            "content": {
                "id": "8200",
                "keywords": [
                    "restaurant"
                ],
                "title": "El Asfour",
                "type": "placeholder",
                "url": "http://www.elmansour-hotel.com/",
                "description": "Route de la Corniche, B.P 217 c/o Iberostar Hotel",
                "geopose": {
                    "position": {
                        "lat": 30.272449917265178,
                        "lon": 59.93412254501339,
                        "h": 0.03469199314713478
                    },
                    "quaternion": {
                        "w": 0.016058681685810556,
                        "x": -0.2691833793848679,
                        "y": -0.00547431425724175,
                        "z": -0.9629394886953536
                    }
                },
                "ecefPose": {
                    "position": {
                        "x": 2766628.8216932337,
                        "y": 1614903.290264482,
                        "z": 5496803.750131461
                    },
                    "orientation": {
                        "w": 0.2471503564975939,
                        "x": -0.011341903574778505,
                        "y": 0.8339283381499026,
                        "z": -0.4933067902806059
                    }
                }
            }
        }
    ]
}
```

### Examples
#### Projects using GeoPose localization
Open AR Cloud WebXR client: https://github.com/OpenArCloud/sparcl

Open AR Cloud Unity client: https://github.com/OpenArCloud/oscp-unity-client

Open AR Cloud Cesium demo (outdated): https://github.com/OpenArCloud/browser_userclient/tree/Cesium



#### Try using with Cesium Sandcastle (outdated)
cesium.com allows you to see the result of work in live coding mode by link https://sandcastle.cesium.com/
just paste the example below into the editor and press "Run (F8)" button
See https://cesium.com/docs/code-examples/

__Description of fields in js example__
```
position - Taken from geopose.ecef.x, geopose.ecef.y, geopose.ecef.z of the /scrs/geopose_objs_local response
orientation - Taken from geopose.ecef.quaternion of the /scrs/geopose_objs_local response
```

```javascript
var viewer = new Cesium.Viewer("cesiumContainer", {
  infoBox: false,
  selectionIndicator: false,
  shadows: true,
  shouldAnimate: true,
});

function createModel(url, height) {
  viewer.entities.removeAll();

  var position = Cesium.Cartesian3.fromElements(2766587.0855982956,  1614763.59540361, 5496865.242906623);
  var orientation = new Cesium.Quaternion(0.2661153288127459, 0.040328282280189406, 0.9231699371248662, -0.2744331040674282);
  var entity = viewer.entities.add({
    name: url,
    position: position,
    orientation: orientation,
    model: {
      uri: url,
      minimumPixelSize: 128,
      maximumScale: 20000,
    },
  });
  viewer.trackedEntity = entity;
}

var options = [
  {
    text: "Aircraft",
    onselect: function () {
      createModel(
        "../SampleData/models/CesiumAir/Cesium_Air.glb",
        5000.0
      );
    },
  }
];

Sandcastle.addToolbarMenu(options);
```