#!/bin/bash
CUR_DIR=$(pwd)

# Set these variables:
OSCP_INSTALL_DIR=$CUR_DIR/install
OSCP_BUILD_DIR=$CUR_DIR/build

CMAKE_EXECUTABLE="cmake"
GENERATOR_NAME="Unix\ makefiles"
GENERATOR_PLATFORM="x64"



mkdir -p $OSCP_BUILD_DIR
mkdir -p $OSCP_INSTALL_DIR
mkdir -p $CUR_DIR/thirdparty


# nlohmann::json
cd $CUR_DIR/thirdparty
git clone https://github.com/nlohmann/json.git
SOURCE_DIR=$CUR_DIR/thirdparty/json
BUILD_DIR=$OSCP_BUILD_DIR/json
cd $SOURCE_DIR
git checkout v3.11.2
mkdir -p $BUILD_DIR
cd $BUILD_DIR
$CMAKE_EXECUTABLE $SOURCE_DIR \
  -DCMAKE_BUILD_TYPE=Release \
  -DCMAKE_INSTALL_PREFIX=$OSCP_INSTALL_DIR \
  -DJSON_BuildTests=OFF \
  -DJSON_MultipleHeaders=OFF
$CMAKE_EXECUTABLE --build . --config Release
$CMAKE_EXECUTABLE --build . --config Release --target install

# cpp-httplib
cd $CUR_DIR/thirdparty
git clone https://github.com/yhirose/cpp-httplib.git
SOURCE_DIR=$CUR_DIR/thirdparty/cpp-httplib
BUILD_DIR=$OSCP_BUILD_DIR/cpp-httplib
cd $SOURCE_DIR
git checkout v0.13.0
mkdir -p $BUILD_DIR
cd $BUILD_DIR
$CMAKE_EXECUTABLE $SOURCE_DIR \
  -DCMAKE_BUILD_TYPE=Release \
  -DCMAKE_INSTALL_PREFIX=$OSCP_INSTALL_DIR
$CMAKE_EXECUTABLE --build . --config Release
$CMAKE_EXECUTABLE --build . --config Release --target install


# STDUUID
cd $CUR_DIR/thirdparty
git clone https://github.com/mariusbancila/stduuid.git
SOURCE_DIR=$CUR_DIR/thirdparty/stduuid
BUILD_DIR=$OSCP_BUILD_DIR/stduuid
cd $SOURCE_DIR
git checkout v1.2.3
mkdir -p $BUILD_DIR
cd $BUILD_DIR
$CMAKE_EXECUTABLE $SOURCE_DIR \
  -DCMAKE_BUILD_TYPE=Release \
  -DCMAKE_INSTALL_PREFIX=$OSCP_INSTALL_DIR \
  -DUUID_BUILD_TESTS=OFF \
  -DUUID_SYSTEM_GENERATOR=OFF \
  -DUUID_TIME_GENERATOR=OFF \
  -DUUID_USING_CXX20_SPAN=OFF \
  -DUUID_ENABLE_INSTALL=ON
$CMAKE_EXECUTABLE --build . --config Release
$CMAKE_EXECUTABLE --build . --config Release --target install

cd $CUR_DIR
