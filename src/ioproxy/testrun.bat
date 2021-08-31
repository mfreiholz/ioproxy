@echo off

set PATH=D:\Qt\5.15.1\msvc2019_64\bin;%PATH%
set QTDIR=D:\Qt\5.15.1\msvc2019_64\bin
set EXEC=".\x64\Debug\ioproxy"

rem goto udpclient

:server
start "SERVER TCP+UDP :13370" %EXEC% ^
	-io tcpserver -id tcpsrv1 -p bind_port=13370 ^
	-io udpsocket -id udpsrv1 -p bind_address=127.0.0.1 -p bind_port=13370 ^
	-io stdout -id stdout1 ^
	--connect tcpsrv1,stdout1 ^
	--connect udpsrv1,stdout1

timeout /t 2

:tcpclient
start "CLIENT TcpSocketIO" %EXEC% ^
	-io text -id txt1 -p "text=data(tcp)" -p interval=250 -p linebreak=1 -p counter=1 -p datetime=0 ^
	-io tcpsocket -id tcpsocket1 -p remote_address=127.0.0.1 -p remote_port=13370 ^
	--connect txt1,tcpsocket1


:udpclient
start "CLIENT UdpSocketIO" %EXEC% ^
	-io text -id txt2 -p "text=data(udp)" -p interval=25 -p linebreak=1 -p counter=1 -p datetime=0 ^
	-io udpsocket -id udp3 -p remote_address=127.0.0.1 -p remote_port=13370 ^
	--connect txt2,udp3
