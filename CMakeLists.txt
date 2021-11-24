cmake_minimum_required(VERSION 3.16)
project(ioproxy-project VERSION 1.0.0)

# Qt Installation Path
if(WIN32)
	set(CMAKE_PREFIX_PATH "$ENV{QTDIR}")
endif()

# Linked dependencies
include(cmake/HumbleLogging.thirdparty.cmake)
include(cmake/LibMUL.thirdparty.cmake)

# Generate version.hpp
configure_file(
	${PROJECT_SOURCE_DIR}/src/ioproxy/include/ioproxy/Version.hpp.in
	${PROJECT_SOURCE_DIR}/src/ioproxy/include/ioproxy/Version.hpp
)

# Sources & Projects
add_subdirectory(src/ioproxy)
add_subdirectory(src/ioproxy-cli)
add_subdirectory(src/ioproxy-gui)
add_subdirectory(src/ioproxy-serialports)
add_subdirectory(src/tcpsocket-data-gen)