#!/bin/bash
CUR_DIR=$(pwd)

# Set these variables:
IMAGE_PATH=$CUR_DIR/../data/seattle.jpg
CAMERA_PARAMS_PATH=$CUR_DIR/../data/seattle_camera_params.json
GEOLOCATION_PARAMS_PATH=$CUR_DIR/../data/seattle_geolocation_params.json

python demo_client.py --image $IMAGE_PATH --camera $CAMERA_PARAMS_PATH --geolocation $GEOLOCATION_PARAMS_PATH
