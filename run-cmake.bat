@echo off
cd %~dp0
mkdir build-debug
cd build-debug

cmake .. -DCMAKE_BUILD_TYPE=Debug -DCMAKE_INSTALL_PREFIX=INSTALL -DBuildShared=OFF -DBuildApps=OFF -DBuildApps=OFF

pause