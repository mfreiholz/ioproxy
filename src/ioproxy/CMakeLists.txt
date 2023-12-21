qt_standard_project_setup()
set(name "ioproxy")

set(headers
	Global.hpp
	Config.hpp
	Engine.hpp
	Factory.hpp
	Handler.hpp
	HumanReadableFormatter.hpp
	IOBase.hpp
	Logging.hpp
	Util.hpp
	io/TcpServerIO.hpp
	io/TcpSocketIO.hpp
	io/UdpSocketIO.hpp
	io/StdOutIO.hpp
	io/FileReaderIO.hpp
	io/FileWriterIO.hpp
	io/SerialPortIO.hpp
	io/CompressionIO.hpp
	os/ServiceRunner.hpp
	os/SignalHandler.hpp
	license/License.hpp
)

set(sources
	Config.cpp
	Engine.cpp
	Factory.cpp
	Handler.cpp
	HumanReadableFormatter.cpp
	Logging.cpp
	Util.cpp
	io/TcpServerIO.cpp
	io/TcpSocketIO.cpp
	io/UdpSocketIO.cpp
	io/StdOutIO.cpp
	io/FileReaderIO.cpp
	io/FileWriterIO.cpp
	io/SerialPortIO.cpp
	io/CompressionIO.cpp
	os/SignalHandler.linux.cpp
	os/SignalHandler.win32.cpp
	license/License.cpp
)

add_library(${name} STATIC)
target_sources(${name} PUBLIC ${headers})
target_sources(${name} PRIVATE ${sources})
target_include_directories(${name} PUBLIC ${CMAKE_CURRENT_LIST_DIR})

target_link_libraries(
	${name}
	Qt6::Core
	Qt6::Network
	Qt6::SerialPort
	Qt6::WebSockets
	Qt6::Concurrent
	humblelogging
)
