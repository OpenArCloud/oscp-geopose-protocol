#!/bin/bash
CUR_DIR=$(pwd)

# Set these variables:
OSCP_INSTALL_DIR=$CUR_DIR/install
OSCP_BUILD_DIR=$CUR_DIR/build

CMAKE_EXECUTABLE="cmake"
GENERATOR_NAME="Unix\ makefiles"
GENERATOR_PLATFORM="x64"


cd $CUR_DIR/oscp-gpp
SOURCE_DIR=$CUR_DIR/oscp-gpp
BUILD_DIR=$OSCP_BUILD_DIR/oscp-gpp
mkdir -p $BUILD_DIR
cd $BUILD_DIR
$CMAKE_EXECUTABLE $SOURCE_DIR \
  -DCMAKE_BUILD_TYPE=Release \
  -DCMAKE_PREFIX_PATH=$OSCP_INSTALL_DIR \
  -DCMAKE_INSTALL_PREFIX=$OSCP_INSTALL_DIR
$CMAKE_EXECUTABLE --build . --config Release
$CMAKE_EXECUTABLE --build . --config Release --target install

cd $CUR_DIR
