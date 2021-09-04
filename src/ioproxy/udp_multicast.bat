@echo off

set PATH=D:\Qt\5.15.1\msvc2019_64\bin;%PATH%
set QTDIR=D:\Qt\5.15.1\msvc2019_64\bin
set EXEC=".\x64\Debug\ioproxy"

%EXEC% ^
-io stdout -name stdout ^
-io text -name txt -p "text=Multicast(HOST)" -p interval=1000 -p linebreak=1 -p counter=1 -p datetime=0 ^
-io udpsocket -name udp_mc24001 -p bind_address=172.25.64.1 -p remote_address=224.10.10.10 -p remote_port=24001 -p multicast_ttl=10 ^
--connect txt,udp_mc24001 ^
--connect udp_mc24001,stdout

pause