cmake_minimum_required(VERSION 3.16)
project(ioproxy-project)

# Qt Installation Path
if(WIN32)
	set(CMAKE_PREFIX_PATH "$ENV{QTDIR}")
endif()

# Linked dependencies
include(cmake/HumbleLogging.thirdparty.cmake)
include(cmake/LibMUL.thirdparty.cmake)

#add_subdirectory(thirdparty/humblelogging)
#include_directories(thirdparty/humblelogging/include)

# Sources & Projects
add_subdirectory(src/ioproxy)
add_subdirectory(src/ioproxy-cli)