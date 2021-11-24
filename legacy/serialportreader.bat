@echo off
set PATH=D:\Qt\5.15.1\msvc2019_64\bin;%PATH%
set QTDIR=D:\Qt\5.15.1\msvc2019_64\bin
set EXEC=".\x64\Debug\ioproxy"

GOTO udp_multicast_sender

:serialport
%EXEC% ^


:udp_multicast_sender
%EXEC% ^
-io udpsocket -name udpmc -p bind_address=172.25.64.1 -p remote_address=224.10.10.10 -p remote_port=24001 -p multicast_ttl=4 ^
-io text -name text -p "text=ABCDEFGHIJKLMNOPQRSTUVWXYZ" -p interval=5 -p counter=1 -p linebreak=0 --connect text,udpmc

:udp_multicast_receiver_and_forwarder
%EXEC% ^
-io udpsocket -name udpmc -p bind_address=172.25.74.159 -p remote_address=224.10.10.10 -p remote_port=24001 -p multicast_ttl=4 ^
-io tcpserver -name tcpserver -p bind_address=172.25.74.159 -p bind_port=13370 -p max_clients=5 ^
--connect udpmc,tcpserver

pause