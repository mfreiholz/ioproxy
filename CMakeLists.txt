cmake_minimum_required(VERSION 3.18)
project(ioproxy-project)

# Qt Installation Path
#set(CMAKE_PREFIX_PATH "$ENV{QTDIR}")

# Linked dependencies
include(cmake/HumbleLogging.thirdparty.cmake)
include(cmake/LibMUL.thirdparty.cmake)

#add_subdirectory(thirdparty/humblelogging)
#include_directories(thirdparty/humblelogging/include)

# Sources & Projects
add_subdirectory(src/ioproxy)