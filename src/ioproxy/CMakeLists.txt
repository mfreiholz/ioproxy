set(name "ioproxy")
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_AUTOUIC ON)
find_package(Qt5 COMPONENTS Core Network SerialPort WebSockets Concurrent REQUIRED)

set(headers_dir ${CMAKE_CURRENT_LIST_DIR}/include/${name})
set(sources_dir ${CMAKE_CURRENT_LIST_DIR}/src)

set(sources
	${headers_dir}/Config.hpp
	${sources_dir}/Config.cpp

	${headers_dir}/Engine.hpp
	${sources_dir}/Engine.cpp

	${headers_dir}/Factory.hpp
	${sources_dir}/Factory.cpp

	${headers_dir}/Global.hpp

	${headers_dir}/Handler.hpp
	${sources_dir}/Handler.cpp

	${headers_dir}/HumanReadableFormatter.hpp
	${sources_dir}/HumanReadableFormatter.cpp

	${headers_dir}/IOBase.hpp

	${headers_dir}/Logging.hpp
	${sources_dir}/Logging.cpp

	${headers_dir}/Util.hpp
	${sources_dir}/Util.cpp

	# /io
	${headers_dir}/io/TcpServerIO.hpp
	${sources_dir}/io/TcpServerIO.cpp
	
	${headers_dir}/io/TcpSocketIO.hpp
	${sources_dir}/io/TcpSocketIO.cpp

	${headers_dir}/io/UdpSocketIO.hpp
	${sources_dir}/io/UdpSocketIO.cpp

	${headers_dir}/io/StdOutIO.hpp
	${sources_dir}/io/StdOutIO.cpp

	${headers_dir}/io/FileWriterIO.hpp
	${sources_dir}/io/FileWriterIO.cpp

	${headers_dir}/io/SerialPortIO.hpp
	${sources_dir}/io/SerialPortIO.cpp

	# /os
	${headers_dir}/os/ServiceRunner.hpp
	${headers_dir}/os/SignalHandler.hpp
	${sources_dir}/os/SignalHandler.linux.cpp
	${sources_dir}/os/SignalHandler.win32.cpp

	# /license
	${headers_dir}/license/License.hpp
	${sources_dir}/license/License.cpp
)
source_group(TREE ${CMAKE_CURRENT_LIST_DIR} FILES ${sources})

add_library(
	${name}
	STATIC
	${sources}
)

target_include_directories(
	${name}
	PUBLIC include
)

target_link_libraries(
	${name}
	Qt5::Core
	Qt5::Network
	Qt5::SerialPort
	Qt5::WebSockets
	Qt5::Concurrent
	${HumbleLogging_LIBRARIES}
)
add_dependencies(${name} HumbleLogging)
add_dependencies(${name} LibMUL)