@echo off
set CUR_DIR=%~dp0

:: Set these variables:
set CONFIG_PATH=%CUR_DIR%\..\data\seattle_vps.json

python.exe demo_server.py --config %CONFIG_PATH%
