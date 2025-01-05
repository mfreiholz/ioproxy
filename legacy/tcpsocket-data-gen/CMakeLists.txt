cmake_minimum_required(VERSION 3.11)
project(tcpsocket-data-gen VERSION 2021.9.1 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_AUTOUIC ON)
find_package(Qt5 COMPONENTS Core Network REQUIRED)

set(sources
	src/main.cpp
)

add_executable(
	${PROJECT_NAME}
	${sources}
)

target_link_libraries(
	${PROJECT_NAME}
	Qt5::Core
	Qt5::Network
	humblelogging
	ioproxy
)

#add_dependencies(${PROJECT_NAME} HumbleLogging)
#add_dependencies(${PROJECT_NAME} LibMUL)
#add_dependencies(${PROJECT_NAME} ioproxy)

# Installation
if(WIN32)
	get_target_property(_qmake_executable Qt5::qmake IMPORTED_LOCATION)
	get_filename_component(_qt_bin_dir "${_qmake_executable}" DIRECTORY)
	find_program(WINDEPLOYQT_EXECUTABLE windeployqt HINTS "${_qt_bin_dir}")
	find_program(MACDEPLOYQT_EXECUTABLE macdeployqt HINTS "${_qt_bin_dir}")
	add_custom_command(
		TARGET ${PROJECT_NAME} POST_BUILD
		COMMAND "${CMAKE_COMMAND}" -E env PATH="${_qt_bin_dir}" "${WINDEPLOYQT_EXECUTABLE}" "$<TARGET_FILE:${PROJECT_NAME}>"
		COMMENT "Running windeployqt..."
	)
endif(WIN32)