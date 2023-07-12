@echo off

set CUR_DIR=%~dp0

:: Set these variables:
set OSCP_INSTALL_DIR=%CUR_DIR%\install
set CONFIG_PATH=%CUR_DIR%\..\data\seattle_vps.json

set PATH=%OSCP_INSTALL_DIR%/bin;%OSCP_INSTALL_DIR%/x64/vc16/bin;%PATH%


cd %OSCP_INSTALL_DIR%\bin
oscp-gpp-server.exe %CONFIG_PATH%
