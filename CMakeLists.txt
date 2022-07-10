cmake_minimum_required(VERSION 3.16)
string(TIMESTAMP UNIX_TIMESTAMP "%s" UTC)

project(
	ioproxy-project
	VERSION 1.0.0.${UNIX_TIMESTAMP}
	DESCRIPTION ""
	HOMEPAGE_URL "https://github.com/mfreiholz/ioproxy"
	LANGUAGES CXX
)

# Qt Installation Path
if(WIN32)
	if(DEFINED ENV{IOPROXY_QTDIR})
		message("Overwrite QTDIR with IOPROXY_QTDIR: $ENV{IOPROXY_QTDIR}")
		set(ENV{QTDIR} "$ENV{IOPROXY_QTDIR}")
	endif()
	set(CMAKE_PREFIX_PATH "$ENV{QTDIR}")
endif()

# Linked dependencies
include(cmake/HumbleLogging.thirdparty.cmake)
include(cmake/LibMUL.thirdparty.cmake)

# Generate version.hpp
configure_file(
	${PROJECT_SOURCE_DIR}/cmake/Version.hpp.in
	${PROJECT_SOURCE_DIR}/src/ioproxy/include/ioproxy/Version.hpp
)

# Sources & Projects
include(cmake/Compiler.cmake)
add_subdirectory(src/ioproxy)
add_subdirectory(src/ioproxy-cli)
add_subdirectory(src/ioproxy-gui)
add_subdirectory(src/ioproxy-serialports)
add_subdirectory(src/tcpsocket-data-gen)