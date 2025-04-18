#!/bin/bash
CUR_DIR=$(pwd)

# Set these variables:
OSCP_INSTALL_DIR=$CUR_DIR/install
VPS_URL=http://127.0.0.1
VPS_PORT=8080
IMAGE_PATH=$CUR_DIR/../data/seattle.jpg
CAMERA_PARAMS_PATH=$CUR_DIR/../data/seattle_camera_params.json
GEOLOCATION_PARAMS_PATH=$CUR_DIR/../data/seattle_geolocation_params.json

# These are only required for running
export LD_LIBRARY_PATH=$OSCP_INSTALL_DIR/lib:$LD_LIBRARY_PATH
export PATH=$OSCP_INSTALL_DIR/bin:$PATH

cd $OSCP_INSTALL_DIR/bin
oscp-gpp-client $VPS_URL $VPS_PORT $IMAGE_PATH $CAMERA_PARAMS_PATH $GEOLOCATION_PARAMS_PATH
