qt_standard_project_setup()
set(name "ioproxy")

set(headers
	Global.hpp
	Engine.hpp
	Handler.hpp
	Version.hpp

	include/${name}/PluginAPI.hpp
	
	io/IOBase.hpp
	io/IOFactoryBase.hpp
	io/TcpServerIO.hpp
	io/TcpSocketIO.hpp
	io/UdpSocketIO.hpp
	io/StdOutIO.hpp
#	io/FileReaderIO.hpp
#	io/FileWriterIO.hpp
	io/SerialPortIO.hpp
#	io/CompressionIO.hpp
	io/WebSocketIO.hpp
	io/WebSocketServerIO.hpp

	os/ServiceRunner.hpp
	os/SignalHandler.hpp

	util/HumanReadableFormatter.hpp
	util/Util.hpp
)

set(sources
	Engine.cpp
	Handler.cpp
	
	io/IOFactoryBase.cpp
	io/TcpServerIO.cpp
	io/TcpSocketIO.cpp
	io/UdpSocketIO.cpp
	io/StdOutIO.cpp
#	io/FileReaderIO.cpp
#	io/FileWriterIO.cpp
	io/SerialPortIO.cpp
#	io/CompressionIO.cpp
	io/WebSocketIO.cpp
	io/WebSocketServerIO.cpp

	os/SignalHandler.linux.cpp
	os/SignalHandler.win32.cpp

	util/HumanReadableFormatter.cpp
	util/Util.cpp
)

add_library(${name} STATIC)
target_sources(${name} PRIVATE ${headers})
target_sources(${name} PRIVATE ${sources})
source_group(TREE ${CMAKE_CURRENT_SOURCE_DIR} PREFIX "Files" FILES ${headers} ${sources})
target_include_directories(${name} PUBLIC "${CMAKE_CURRENT_SOURCE_DIR}/..")
target_include_directories(${name} PUBLIC "${CMAKE_CURRENT_SOURCE_DIR}/include")

target_link_libraries(
	${name}
	PUBLIC
		Qt6::Core
		Qt6::Network
		Qt6::SerialPort
		Qt6::WebSockets
		Qt6::Concurrent
		humblelogging
		tl-expected
)