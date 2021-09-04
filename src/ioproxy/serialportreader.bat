@echo off

set PATH=D:\Qt\5.15.1\msvc2019_64\bin;%PATH%
set QTDIR=D:\Qt\5.15.1\msvc2019_64\bin
set EXEC=".\x64\Debug\ioproxy"

start "Serial Port COM1 Reader" %EXEC% ^
	-io serialport -name com1 -p port=COM1 ^
	-io stdout -name stdout1 ^
	--connect com1,stdout1

pause