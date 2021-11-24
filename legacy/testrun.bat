@echo off

set PATH=D:\Qt\5.15.1\msvc2019_64\bin;%PATH%
set QTDIR=D:\Qt\5.15.1\msvc2019_64\bin
set EXEC=".\src\ioproxy\x64\Debug\ioproxy"

goto tcp_client

:tcp_udp_server
%EXEC% ^
	-io tcpserver -name tcpserver -p bind_port=13370 -p broadcast_clients=1 -p max_clients=10 ^
	-io udpsocket -name udpsocket -p bind_address=127.0.0.1 -p bind_port=13370 ^
	-io stdout -name stdout ^
	--connect tcpserver,stdout ^
	--connect udpsocket,stdout
pause

:tcp_client
%EXEC% ^
	-io text -name text -p "text=data(tcp)" -p interval=25 -p linebreak=1 -p counter=1 -p datetime=1 ^
	-io tcpsocket -name tcpsocket -p remote_address=127.0.0.1 -p remote_port=13370 ^
	--connect text,tcpsocket
pause

:udpclient
start "CLIENT UdpSocketIO" %EXEC% ^
	-io text -name txt2 -p "text=data(udp)" -p interval=25 -p linebreak=1 -p counter=1 -p datetime=0 ^
	-io udpsocket -name udp3 -p remote_address=127.0.0.1 -p remote_port=13370 ^
	--connect txt2,udp3
pause

:udp_multicast
start "udp_multicast" %EXEC% ^
	-io stdout -name stdout ^
	-io text -name text -p "text=udp_multicast" -p linebreak=1 -p interval=1000 -p counter=1 -p datetime=1 ^
	-io udpsocket -name udp_mc24001 -p bind_address=172.25.64.1 -p remote_address=224.10.10.10 -p remote_port=24001 -p multicast_ttl=10 ^
	--connect text,udp_mc24001 ^
	--connect udp_mc24001,stdout
pause

:serialport
start "serialport" %EXEC% ^
	-io stdout -name out ^
	-io text -name text -p "text=serialport(COM1)" -p linebreak=1 -p interval=1000 -p counter=1 -p datetime=1 ^
	-io serialport -name serialport -p port=COM1 -p baudrate=115200 ^
	--connect text,serial --connect serial,out
pause

:end
echo >
echo Bye Bye
echo >