cmake_minimum_required(VERSION 3.16)
string(TIMESTAMP UNIX_TIMESTAMP "%s" UTC)

project(
	IO_Proxy_Project
	VERSION 1.0.0.${UNIX_TIMESTAMP}
	DESCRIPTION ""
	HOMEPAGE_URL "https://github.com/mfreiholz/ioproxy"
	LANGUAGES CXX
)

# Generate version.hpp
configure_file(
	${PROJECT_SOURCE_DIR}/cmake/Version.hpp.in
	${PROJECT_SOURCE_DIR}/src/ioproxy/include/ioproxy/Version.hpp
)

# Compiler Settings.
include(cmake/Compiler.cmake)

# Third party libraries.
include(thirdparty/Qt.cmake)
include(thirdparty/humblelogging.cmake)

include_directories(src)
add_subdirectory(src/ioproxy)
add_subdirectory(src/ioproxy-cli)
add_subdirectory(src/ioproxy-gui)
add_subdirectory(src/ioproxy-gui-widgets)
#add_subdirectory(src/ioproxy-serialports)
#add_subdirectory(src/tcpsocket-data-gen)
#add_subdirectory(src/udpsocket-data-gen)