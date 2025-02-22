cmake_minimum_required(VERSION 3.16)
string(TIMESTAMP UNIX_TIMESTAMP "%s" UTC)

project(
	IO_Proxy_Project
	VERSION 1.0.0.${UNIX_TIMESTAMP}
	DESCRIPTION ""
	HOMEPAGE_URL "https://github.com/mfreiholz/ioproxy"
	LANGUAGES CXX
)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
cmake_policy(SET CMP0174 OLD)

# Generate version.hpp
configure_file(
	${PROJECT_SOURCE_DIR}/cmake/Version.hpp.in
	${PROJECT_SOURCE_DIR}/src/ioproxy/Version.hpp
)

# Third party libraries.
include(thirdparty/Qt.cmake)
include(thirdparty/humblelogging.cmake)
include(thirdparty/tl-expected.cmake)

# Source.
add_subdirectory(src/ioproxy)
add_subdirectory(src/ioproxy-cli)
