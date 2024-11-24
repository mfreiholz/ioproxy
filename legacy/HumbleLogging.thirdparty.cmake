include(ExternalProject)

ExternalProject_Add(
	HumbleLogging

	GIT_REPOSITORY https://github.com/mfreiholz/humblelogging.git
	GIT_TAG "master"

	CMAKE_ARGS -DBuildShared=OFF -DBuildExamples=OFF -DBuildSystemNative=ON

	INSTALL_COMMAND ""
	TEST_COMMAND ""
)
ExternalProject_Get_Property(HumbleLogging SOURCE_DIR)
ExternalProject_Get_Property(HumbleLogging BINARY_DIR)
message("HumbleLogging SOURCE_DIR = ${SOURCE_DIR}")
message("HumbleLogging BINARY_DIR = ${BINARY_DIR}")

#ExternalProject_Add_Step(
#	HumbleLogging CopyToBin
#	COMMAND ${CMAKE_COMMAND} -E copy_directory ${GLOBAL_OUTPUT_PATH}/humblelogging/bin ${GLOBAL_OUTPUT_PATH}
#	COMMAND ${CMAKE_COMMAND} -E copy_directory ${GLOBAL_OUTPUT_PATH}/humblelogging/lib ${GLOBAL_OUTPUT_PATH}
#	DEPENDEES install
#)

set(HumbleLogging_INCLUDE_DIRS "${SOURCE_DIR}/include")
include_directories(${HumbleLogging_INCLUDE_DIRS})

if(WIN32)
	set(HumbleLogging_LIBRARIES
		optimized "${BINARY_DIR}/Release/humblelogging.lib"
		debug "${BINARY_DIR}/Debug/humblelogging.lib"
	)
else()
	set(HumbleLogging_LIBRARIES
		"${BINARY_DIR}/libhumblelogging.a"
	)
endif()