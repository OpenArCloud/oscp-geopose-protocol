# Geopose and objects pose endpoints
Endpoints are available at https://developer.augmented.city/
### Usage
A geo-pose represents the object's coordinate system position in the world coordinate system, so that it is accurately positioned on the globe, correctly oriented with respect to cardinal directions and vertical axis.  
There can be many options with different world coordinate system: ECEF or Local Tangent Plane systems and others (https://en.wikipedia.org/wiki/Local_tangent_plane_coordinates)  
And also a number of object’s representations.  
Geo-pose consists of rotation part (quaternion) and position part.  
By applying these transformations to the points of the object model (rotation first, then translation to position), one can calculate their location in the world.

__Description of geopose and scrs fields__

Geopose called “ecef“ is intended to place objects in ECEF coordinate system which for example can be directly applied to Cesium platform.
The object's frame should be oriented with +X forward direction and +Z in upwards prior to applying geopose transformations. If it is not, consider to combine the geopose with pre-rotations (using quaternion multiplication routines)

Geopose called “pose“ is an Local Tangent Plane transformation, which means it places the object in familiar cartesian world with axes aligned to cardinal directions. But it is implied that the object’s origin is located exactly in coordinates "latitude", "longitude", "altitude".
This realization can be used by AR-enabled tools. It deals with object and world coordinate systems specific to current ARKit/ARCore. Object’s forward direction towards -Z and Up direction is +Y. World’s X points east, Y up, Z south


#### /scrs/geopose_objs - get geopose with objects in ARKit/ARCore coordinates and ECEF coordinates

```
geopose.ecef.quaternion - rotation transformation (ECEF coordinate system)
geopose.ecef.x - The object’s origin coordinates relative to Earth’s mass center (ECEF coordinate system), units - meters.
geopose.ecef.y - The object’s origin coordinates relative to Earth’s mass center (ECEF coordinate system), units - meters.
geopose.ecef.z - The object’s origin coordinates relative to Earth’s mass center (ECEF coordinate system), units - meters.
The WGS84 ellipsoid level is treated as zero height

geopose.pose.altitude - camera’s origin height about ground level
geopose.pose.latitude - geodetic coordinates
geopose.pose.longitude - geodetic coordinates
geopose.pose.quaternion - rotation transformation(ARKit/ARCore coordinate system)

scrs[0].content.geopose.ecef.quaternion - rotation transformation (ECEF coordinate system)
scrs[0].content.geopose.ecef.x - The object’s origin coordinates relative to Earth’s mass center (ECEF coordinate system), units - meters.
scrs[0].content.geopose.ecef.y - The object’s origin coordinates relative to Earth’s mass center (ECEF coordinate system), units - meters.
scrs[0].content.geopose.ecef.z - The object’s origin coordinates relative to Earth’s mass center (ECEF coordinate system), units - meters.

scrs[0].content.geopose.altitude - object’s origin height about ground level
scrs[0].content.geopose.latitude - geodetic coordinates
scrs[0].content.geopose.longitude - geodetic coordinates    
scrs[0].content.geopose.quaternion - rotation transformation(object coordinate system to ARKit/ARCore coordinate system)
```
__Response example__
```json
{
    "geopose": {
        "accuracy": -1,
        "ecef": {
            "quaternion": [
                0.2661153288127459,
                0.040328282280189406,
                0.9231699371248662,
                -0.2744331040674282
            ],
            "x": 2766587.0855982956,
            "y": 1614763.59540361,
            "z": 5496865.242906623
        },
        "id": "9089876676575754",
        "pose": {
            "altitude": -0.08380349220203223,
            "ellipsoidHeight": -1,
            "latitude": 59.93522609622502,
            "longitude": 30.27066834299855,
            "quaternion": [
                -0.007090709830811782,
                -0.031015972010745515,
                -0.02327018097625009,
                -0.9992228129860009
            ]
        },
        "timestamp": "Thu, 10 Dec 2020 17:14:03 GMT",
        "type": "geopose"
    },
    "scrs": [
        {
            "content": {
                "description": "Route de la Corniche, B.P 217 c/o Iberostar Hotel",
                "geopose": {
                    "altitude": 0.03469199314713478,
                    "ecef": {
                        "quaternion": [
                            0.2471503564975939,
                            -0.011341903574778505,
                            0.8339283381499026,
                            -0.4933067902806059
                        ],
                        "x": 2766628.8216932337,
                        "y": 1614903.290264482,
                        "z": 5496803.750131461
                    },
                    "ellipsoidHeight": -1,
                    "latitude": 30.272449917265178,
                    "longitude": 59.93412254501339,
                    "quaternion": [
                        0.016058681685810556,
                        -0.2691833793848679,
                        -0.00547431425724175,
                        -0.9629394886953536
                    ]
                },
                "id": "8200",
                "keywords": [
                    "restaurant"
                ],
                "title": "El Asfour",
                "type": "placeholder",
                "url": "http://www.elmansour-hotel.com/"
            },
            "id": "8200",
            "tenant": "AC",
            "timestamp": "2020-12-10T17:14:01.795301",
            "type": "scr"
        }
    ]
}
```
#### /scrs/geopose - get geopose only in ARKit/ARCore coordinate system

__Description of geopose fields__

```
geopose.pose.altitude - camera’s origin height about ground level
geopose.pose.latitude - geodetic coordinates
geopose.pose.longitude - geodetic coordinates
geopose.pose.quaternion - rotation transformation(object coordinate system to ARKit/ARCore coordinate system)
```
__Response example__
```json
{
    "accuracy": -1,
    "id": "9089876676575754",
    "pose": {
        "altitude": -0.08433365951111288,
        "ellipsoidHeight": -1,
        "latitude": 59.93522601509476,
        "longitude": 30.27066840280691,
        "quaternion": [
            -0.007102287767713538,
            -0.031042676130458968,
            -0.023274419381073084,
            -0.9992218027893498
        ]
    },
    "timestamp": "Fri, 04 Dec 2020 19:41:10 GMT",
    "type": "geopose"
}
```

### Examples
#### Projects using AC geopose
https://github.com/OpenArCloud/browser_userclient/tree/Cesium - sources
https://testing.browsar.app/usr/geopose/localizephoto - live example
#### Try using with Cesium Sandcastle

cesium.com allows you to see the result of work in live coding mode by link https://sandcastle.cesium.com/
just paste the example below into the editor and press "Run (F8)" button
See https://cesium.com/docs/code-examples/

__Description of fields in js example__
```
position - Taken from geopose.ecef.x, geopose.ecef.y, geopose.ecef.z of the /scrs/geopose_objs response
orientation - Taken from geopose.ecef.quaternion of the /scrs/geopose_objs response
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