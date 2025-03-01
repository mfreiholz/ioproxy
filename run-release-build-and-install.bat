@echo off
cd %~dp0
mkdir build-release
cd build-release

cmake .. -DCMAKE_BUILD_TYPE=Release -DCMAKE_INSTALL_PREFIX=INSTALL -DBuildShared=OFF -DBuildApps=OFF -DBuildApps=OFF
if errorlevel 1 goto end

cmake --build . --config Release
if errorlevel 1 goto end

cmake --install . --config Release
if errorlevel 1 goto end

D:\Development\C_Workspace\thirdparty\qt\6.8.0\msvc2022_64\bin\windeployqt.exe^
  --compiler-runtime^
   --release^
  %~dp0build-release\INSTALL\bin\ioproxy-cli.exe
if errorlevel 1 goto end

COPY %~dp0thirdparty\windows_x64\*.dll^
  %~dp0build-release\INSTALL\bin


:end
pause