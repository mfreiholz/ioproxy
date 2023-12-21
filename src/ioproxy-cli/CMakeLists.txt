set(name "ioproxy-cli")
qt_standard_project_setup()

set(sources
	src/CmdLineConfig.hpp
	src/CmdLineConfig.cpp
	src/ConsoleStatusPrinter.hpp
	src/ConsoleStatusPrinter.cpp
	src/IOProxyConsoleApplication.hpp
	src/IOProxyConsoleApplication.cpp
	src/Main.cpp
)

add_executable(
	${name}
	${sources}
)

target_link_libraries(
	${name}
	Qt6::Core
	humblelogging
	ioproxy
)

#add_dependencies(${PROJECT_NAME} HumbleLogging)
#add_dependencies(${PROJECT_NAME} LibMUL)
#add_dependencies(${PROJECT_NAME} ioproxy)

# Installation
#if(WIN32)
#	get_target_property(_qmake_executable Qt5::qmake IMPORTED_LOCATION)
#	get_filename_component(_qt_bin_dir "${_qmake_executable}" DIRECTORY)
#	find_program(WINDEPLOYQT_EXECUTABLE windeployqt HINTS "${_qt_bin_dir}")
#	find_program(MACDEPLOYQT_EXECUTABLE macdeployqt HINTS "${_qt_bin_dir}")
#	add_custom_command(
#		TARGET ${name} POST_BUILD
#		COMMAND "${CMAKE_COMMAND}" -E env PATH="${_qt_bin_dir}" "${WINDEPLOYQT_EXECUTABLE}" "$<TARGET_FILE:${name}>"
#		COMMENT "Running windeployqt..."
#	)
#endif(WIN32)