set CUR_DIR=%~dp0

:: Set these variables:
set OSCP_INSTALL_DIR=%CUR_DIR%\install
set OSCP_BUILD_DIR=%CUR_DIR%\build

set CMAKE_EXECUTABLE="C:\Program Files\CMake\bin\cmake.exe"
set GENERATOR_NAME="Visual Studio 17 2022"
set GENERATOR_PLATFORM="x64"


cd %CUR_DIR%\oscp-gpp
set SOURCE_DIR=%CUR_DIR%\oscp-gpp
set BUILD_DIR=%OSCP_BUILD_DIR%\oscp-gpp
mkdir %BUILD_DIR%
cd %BUILD_DIR%
%CMAKE_EXECUTABLE% %SOURCE_DIR% ^
  -DCMAKE_BUILD_TYPE=Release ^
  -DCMAKE_PREFIX_PATH=%OSCP_INSTALL_DIR% ^
  -DCMAKE_INSTALL_PREFIX=%OSCP_INSTALL_DIR%
%CMAKE_EXECUTABLE% --build . --config Release
%CMAKE_EXECUTABLE% --build . --config Release --target install
