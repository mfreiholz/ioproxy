@echo off
set DebugOrRelease=Release

rem TCP & UDP core servers.
rem These are the final servers which receives all data via proxies.
start ..\build\src\ioproxy-cli\%DebugOrRelease%\ioproxy-cli.exe ^
-io tcpserver -name tcpcore -p bind_address=0.0.0.0 -p bind_port=24001 ^
-io udpsocket -name udpcore -p bind_address=0.0.0.0 -p bind_port=25001 ^
-io stdout -name stdout

rem TCP Proxy
start ..\build\src\ioproxy-cli\%DebugOrRelease%\ioproxy-cli.exe ^
-io tcpserver -name tcp-proxy-in -p bind_address=0.0.0.0 -p bind_port=24002 ^
-io tcpsocket -name tcp-proxy-out -p remote_address=127.0.0.1 -p remote_port=24001 -p reconnect=1 ^
-connect tcp-proxy-in,tcp-proxy-out

rem UDP Proxy
start ..\build\src\ioproxy-cli\%DebugOrRelease%\ioproxy-cli.exe ^
-io udpsocket -name udp-proxy-in -p bind_address=0.0.0.0 -p bind_port=25002 ^
-io udpsocket -name udp-proxy-out -p remote_address=127.0.0.1 -p remote_port=25001 ^
-connect udp-proxy-in,udp-proxy-out

rem Starts data-generator which sends data via tcp to proxy server.
start ..\build\src\tcpsocket-data-gen\%DebugOrRelease%\tcpsocket-data-gen.exe 127.0.0.1 24002

rem Starts UDP generator.
start ..\build\src\udpsocket-data-gen\%DebugOrRelease%\udpsocket-data-gen.exe 127.0.0.1 25002
