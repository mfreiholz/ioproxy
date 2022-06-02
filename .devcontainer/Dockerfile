FROM ubuntu:20.04
ENV DEBIAN_FRONTEND=noninteractive

RUN apt-get update

# Build tools.
RUN apt-get install -y wget
RUN apt-get install -y git
RUN apt-get install -y gcc
RUN apt-get install -y g++
RUN apt-get install -y gdb
RUN apt-get install -y cmake
RUN apt-get install -y clang-format

# Not required, because it comes with the VSCode C++ extension
#RUN apt-get install -y clang-format

# Application dependencies.
RUN apt-get install -y qt5-default
RUN apt-get install -y libqt5serialport5-dev libqt5serialport5
RUN apt-get install -y libqt5websockets5-dev libqt5websockets5
