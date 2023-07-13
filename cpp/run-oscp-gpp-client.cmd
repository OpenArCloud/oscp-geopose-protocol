@echo off

set CUR_DIR=%~dp0

:: Set these variables:
set OSCP_INSTALL_DIR=%CUR_DIR%\install
set IMAGE_PATH=%CUR_DIR%\..\data\seattle.jpg
set CAMERA_PARAMS_PATH=%CUR_DIR%\..\data\seattle_camera_params.json
set GEOLOCATION_PARAMS_PATH=%CUR_DIR%\..\data\seattle_geolocation_params.json

set PATH=%OSCP_INSTALL_DIR%/bin;%OSCP_INSTALL_DIR%/x64/vc16/bin;%PATH%
::echo %PATH%

cd %OSCP_INSTALL_DIR%\bin
oscp-gpp-client.exe %IMAGE_PATH% %CAMERA_PARAMS_PATH% %GEOLOCATION_PARAMS_PATH%
