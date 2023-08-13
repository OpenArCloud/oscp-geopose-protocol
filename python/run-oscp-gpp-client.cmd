@echo off
set CUR_DIR=%~dp0

:: Set these variables:
set IMAGE_PATH=%CUR_DIR%\..\data\seattle.jpg
set CAMERA_PARAMS_PATH=%CUR_DIR%\..\data\seattle_camera_params.json
set GEOLOCATION_PARAMS_PATH=%CUR_DIR%\..\data\seattle_geolocation_params.json

python.exe demo_client.py --image %IMAGE_PATH% --camera %CAMERA_PARAMS_PATH% --geolocation %GEOLOCATION_PARAMS_PATH%
