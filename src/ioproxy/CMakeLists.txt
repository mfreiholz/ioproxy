qt_standard_project_setup()
set(name "ioproxy")

set(files
	"Engine.hpp"
	"Engine.cpp"
	"Global.hpp"
	"Handler.cpp"
	"Handler.hpp"
	"Version.hpp"

	"io/ByteGenerator/ByteGeneratorIO.cpp"
	"io/ByteGenerator/ByteGeneratorIO.hpp"
	"io/ByteGenerator/ByteGeneratorFactory.cpp"
	"io/ByteGenerator/ByteGeneratorFactory.hpp"

	"io/IOBase.hpp"

	"io/IOFactoryBase.cpp"
	"io/IOFactoryBase.hpp"

	"io/SerialPortIO.cpp"
	"io/SerialPortIO.hpp"

	"io/StdOutIO.cpp"
	"io/StdOutIO.hpp"
	
	"io/TcpServerIO.cpp"
	"io/TcpServerIO.hpp"
	
	"io/TcpSocketIO.cpp"
	"io/TcpSocketIO.hpp"

	"io/UdpSocketIO.cpp"
	"io/UdpSocketIO.hpp"
	
	"io/WebSocketIO.cpp"
	"io/WebSocketIO.hpp"
	
	"io/WebSocketServerIO.cpp"
	"io/WebSocketServerIO.hpp"
	
	"os/ServiceRunner.hpp"
	
	"os/SignalHandler.linux.cpp"
	"os/SignalHandler.win32.cpp"
	"os/SignalHandler.hpp"

	"sdk/ioproxy-plugin-sdk.hpp"
	
	"util/HumanReadableFormatter.cpp"
	"util/HumanReadableFormatter.hpp"
	
	"util/Util.cpp"
	"util/Util.hpp"
)

add_library(${name} STATIC)
target_sources(${name} PRIVATE ${files})
source_group(TREE ${CMAKE_CURRENT_SOURCE_DIR} PREFIX "SOURCES" FILES ${files})
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

install(TARGETS ${name} DESTINATION lib)