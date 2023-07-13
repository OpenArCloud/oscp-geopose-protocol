# General

Open AR Cloud GeoPoseProtocol C++ implementation

Created based on the protocol definition: https://github.com/OpenArCloud/oscp-geopose-protocol

Created by Gabor Soros, Nokia Bell Labs, 2022
Copyright Nokia
MIT License

# Building on Windows
1. Install OpenSSL or build yourself it as described in `build-openssl-windows.txt`

2. Build and install the dependencies by running `build-dependencies.cmd` in PowerShell.
You may need to install OpenCV separately, prebuilt packages are available at https://opencv.org/releases/, extract a package into the `OSCP_INSTALL_DIR`.
You may need to add the `%OSCP_INSTALL_DIR%/bin` and the OpenCV DLLs `%OSCP_INSTALL_DIR%/x64/vc16|vc17/bin` to your `Path` environment variable.

3. Build and install the `oscp-gpp` library by running `build-oscp-gpp.cmd` in PowerShell

4. Build and install the demo by running `build-oscp-gpp-demo.cmd` in PowerShell


# Building on Linux
1. Install OpenSSL with `sudo apt install openssl libssl-dev`

2. Build and install the dependencies by running `build-dependencies.sh`

3. Build and install the `oscp-gpp` library by running `build-oscp-gpp.sh`

4. Build and install the demo by running `build-oscp-gpp-demo.sh`

# Example data
The `data` folder contains an example image `seattle.jpg` along with camera parameters `seattle_camera_params.json` and coarse geolocation `seattle_geolocation_params.json`. A corresponding VPS response is provided in `seattle_vps.json`.

# Running on Windows
Start the server: `run-oscp-gpp-server.cmd`. You can test whether the server is running by typing in your browser: `http://localhost:8080/geopose`

Start the client and execute a single request: `run-oscp-gpp-client.cmd`

# Running on Linux
Similar to Windows but run the Shell scripts.
