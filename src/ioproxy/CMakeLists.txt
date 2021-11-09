cmake_minimum_required(VERSION 3.11)
project(ioproxy VERSION 2021.9.1 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_AUTOUIC ON)
find_package(Qt5 COMPONENTS Core Network SerialPort WebSockets Concurrent REQUIRED)

set(sources
	include/${PROJECT_NAME}/App.hpp
	include/${PROJECT_NAME}/AppContext.hpp
	include/${PROJECT_NAME}/CommandLineInitializer.hpp
	include/${PROJECT_NAME}/Global.hpp
	include/${PROJECT_NAME}/IOBase.hpp
	include/${PROJECT_NAME}/IOFactory.hpp
	include/${PROJECT_NAME}/IOHandler.hpp
	include/${PROJECT_NAME}/Util.hpp
	src/Util.cpp
	include/${PROJECT_NAME}/HumanReadableFormatter.hpp
	src/HumanReadableFormatter.cpp

	# io
	include/${PROJECT_NAME}/io/DataGeneratorIO.hpp
	src/io/DataGeneratorIO.cpp
	include/${PROJECT_NAME}/io/FileWriterIO.hpp
	include/${PROJECT_NAME}/io/SerialPortIO.hpp
	include/${PROJECT_NAME}/io/StdOutIO.hpp
	include/${PROJECT_NAME}/io/TcpServerIO.hpp
	include/${PROJECT_NAME}/io/TcpSocketIO.hpp
	include/${PROJECT_NAME}/io/TextIO.hpp
	include/${PROJECT_NAME}/io/UdpSocketIO.hpp
	include/${PROJECT_NAME}/io/WebSocketHandler.hpp
	include/${PROJECT_NAME}/io/WebSocketServerIO.hpp

	# os
	include/${PROJECT_NAME}/os/ServiceRunner.hpp
	include/${PROJECT_NAME}/os/SignalHandler.hpp
	src/os/SignalHandler.linux.cpp
	src/os/SignalHandler.win32.cpp

	# license
	include/${PROJECT_NAME}/license/License.hpp
	src/license/License.cpp
)

add_library(
	${PROJECT_NAME}
	STATIC
	${sources}
)

target_include_directories(
	${PROJECT_NAME}
	PUBLIC include
)

target_link_libraries(
	${PROJECT_NAME}
	Qt5::Core
	Qt5::Network
	Qt5::SerialPort
	Qt5::WebSockets
	Qt5::Concurrent
	${HumbleLogging_LIBRARIES}
)
add_dependencies(${PROJECT_NAME} HumbleLogging)
add_dependencies(${PROJECT_NAME} LibMUL)
