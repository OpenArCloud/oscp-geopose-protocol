set CUR_DIR=%~dp0

:: Set these variables:
set OSCP_INSTALL_DIR=%CUR_DIR%\install
set OSCP_BUILD_DIR=%CUR_DIR%\build

set CMAKE_EXECUTABLE="C:\Program Files\CMake\bin\cmake.exe"
set GENERATOR_NAME="Visual Studio 17 2022"
set GENERATOR_PLATFORM="x64"


mkdir %OSCP_BUILD_DIR%
mkdir %OSCP_INSTALL_DIR%
mkdir %CUR_DIR%\thirdparty

:: nlohmann::json
cd %CUR_DIR%\thirdparty
git clone https://github.com/nlohmann/json.git
set SOURCE_DIR=%CUR_DIR%\thirdparty\json
set BUILD_DIR=%OSCP_BUILD_DIR%\json
cd %SOURCE_DIR%
git checkout v3.11.2
mkdir %BUILD_DIR%
cd %BUILD_DIR%
%CMAKE_EXECUTABLE% %SOURCE_DIR% ^
  -DCMAKE_BUILD_TYPE=Release ^
  -DCMAKE_INSTALL_PREFIX=%OSCP_INSTALL_DIR% ^
  -DJSON_BuildTests=OFF ^
  -DJSON_MultipleHeaders=OFF
%CMAKE_EXECUTABLE% --build . --config Release
%CMAKE_EXECUTABLE% --build . --config Release --target install


:: cpp-httplib
cd %CUR_DIR%\thirdparty
git clone https://github.com/yhirose/cpp-httplib.git
set SOURCE_DIR=%CUR_DIR%\thirdparty\cpp-httplib
set BUILD_DIR=%OSCP_BUILD_DIR%\cpp-httplib
cd %SOURCE_DIR%
git checkout v0.13.0
mkdir %BUILD_DIR%
cd %BUILD_DIR%
%CMAKE_EXECUTABLE% %SOURCE_DIR% ^
  -DCMAKE_BUILD_TYPE=Release ^
  -DCMAKE_INSTALL_PREFIX=%OSCP_INSTALL_DIR% ^
  -DHTTPLIB_REQUIRE_OPENSSL=ON
:: NOTE: we need SSL support, and for that the OpenSSL library needs to be available on our machine.
:: It can be (not entirely trivially) compiled and installed for Windows by following these steps: https://github.com/openssl/openssl/blob/master/NOTES-WINDOWS.md
%CMAKE_EXECUTABLE% --build . --config Release
%CMAKE_EXECUTABLE% --build . --config Release --target install


:: STDUUID
cd %CUR_DIR%\thirdparty
git clone https://github.com/mariusbancila/stduuid.git
set SOURCE_DIR=%CUR_DIR%\thirdparty\stduuid
set BUILD_DIR=%OSCP_BUILD_DIR%\stduuid
cd %SOURCE_DIR%
git checkout v1.2.3
mkdir %BUILD_DIR%
cd %BUILD_DIR%
%CMAKE_EXECUTABLE% %SOURCE_DIR% ^
  -DCMAKE_BUILD_TYPE=Release ^
  -DCMAKE_INSTALL_PREFIX=%OSCP_INSTALL_DIR% ^
  -DUUID_BUILD_TESTS=OFF ^
  -DUUID_SYSTEM_GENERATOR=OFF ^
  -DUUID_TIME_GENERATOR=OFF ^
  -DUUID_USING_CXX20_SPAN=OFF ^
  -DUUID_ENABLE_INSTALL=ON
%CMAKE_EXECUTABLE% --build . --config Release
%CMAKE_EXECUTABLE% --build . --config Release --target install

