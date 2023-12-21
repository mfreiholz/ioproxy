@echo off
set DebugOrRelease=Release

rem Copy file.
start ..\build\src\ioproxy-cli\%DebugOrRelease%\ioproxy-cli.exe ^
-io filereader -name fin -p "file=C:\Users\mfrei\Downloads\OpenShot-v3.0.0-x86_64.exe" ^
-io filewriter -name fout -p "file=C:\Users\mfrei\Downloads\test.exe" ^
-connect fin,fout
