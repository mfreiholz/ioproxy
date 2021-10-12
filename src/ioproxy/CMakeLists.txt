cmake_minimum_required(VERSION 3.11)
project(ioproxy VERSION 2021.9.1 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_AUTOUIC ON)

find_package(Qt5 COMPONENTS Core Network SerialPort WebSockets Concurrent REQUIRED)

#list(APPEND CMAKE_PROGRAM_PATH  "$ENV{QTDIR}")

set(headers
	App.hpp
	AppContext.hpp
	CommandLineInitializer.hpp
	Global.hpp
	IOBase.hpp
	IOFactory.hpp
	IOHandler.hpp
	Util.hpp

	# io
	io/FileWriterIO.hpp
	io/SerialPortIO.hpp
	io/StdOutIO.hpp
	io/TcpServerIO.hpp
	io/TcpSocketIO.hpp
	io/TextIO.hpp
	io/UdpSocketIO.hpp
	#io/WebSocketClientIO.hpp
	io/WebSocketHandler.hpp
	io/WebSocketServerIO.hpp

	# os
	os/ServiceRunner.hpp
	os/SignalHandler.hpp
)

set(sources
	Util.cpp
	main.cpp

	# io
	# ...

	# os
	os/SignalHandler.linux.cpp
	os/SignalHandler.win32.cpp
)

add_executable(
	${PROJECT_NAME}
	#WIN32
	${headers}
	${sources}
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

# Installation
if(WIN32)
	get_target_property(_qmake_executable Qt5::qmake IMPORTED_LOCATION)
	get_filename_component(_qt_bin_dir "${_qmake_executable}" DIRECTORY)
	find_program(WINDEPLOYQT_EXECUTABLE windeployqt HINTS "${_qt_bin_dir}")
	find_program(MACDEPLOYQT_EXECUTABLE macdeployqt HINTS "${_qt_bin_dir}")

	add_custom_command(
		TARGET ${PROJECT_NAME} POST_BUILD
		COMMAND "${CMAKE_COMMAND}" -E env PATH="${_qt_bin_dir}" "${WINDEPLOYQT_EXECUTABLE}" "$<TARGET_FILE:${PROJECT_NAME}>"
		COMMENT "Running windeployqt..."
	)
endif(WIN32)