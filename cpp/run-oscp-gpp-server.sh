#!/bin/bash
CUR_DIR=$(pwd)

# Set these variables:
OSCP_INSTALL_DIR=$CUR_DIR/install
CONFIG_PATH=$CUR_DIR/../data/seattle_vps.json

# These are only required for running
export LD_LIBRARY_PATH=$OSCP_INSTALL_DIR/lib:$LD_LIBRARY_PATH
export PATH=$OSCP_INSTALL_DIR/bin:$PATH

cd $OSCP_INSTALL_DIR/bin
oscp-gpp-server $CONFIG_PATH
