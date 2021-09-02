@echo off

set PATH=D:\Qt\5.15.1\msvc2019_64\bin;%PATH%
set QTDIR=D:\Qt\5.15.1\msvc2019_64\bin
set EXEC=".\x64\Debug\ioproxy"

%EXEC% serialports

pause