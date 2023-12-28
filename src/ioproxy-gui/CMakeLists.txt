set(name "ioproxy-gui")
qt_standard_project_setup()

qt_add_executable(
	${name}

	App.hpp
	App.cpp
	Blueprint.hpp
	Blueprint.cpp
	BlueprintItem.hpp
	BlueprintItem.cpp
	ConfigReader.hpp
	ConfigReader.cpp
	Main.cpp
)

qt_add_resources(
	${name} "qml"
	PREFIX
		"/qml/"
	FILES
		"BlueprintTcpSocket.qml"

		"BlueprintBackground.qml"
		"BlueprintItem.qml"
		"BlueprintView.qml"
		"Constants.qml"
		"Main.qml"
		"App.js"
		"qmldir"
)

qt_add_resources(
	${name} "controls"
	PREFIX
		"/"
	FILES
		"qtquickcontrols2.conf"
)

target_link_libraries(
	${name}
	PRIVATE
		ioproxy
		Qt6::Core
		Qt6::Gui
		Qt6::Widgets
		Qt6::Quick
		Qt6::Qml
		Qt6::Xml
		humblelogging
)

set_target_properties(
	${name} PROPERTIES
		VS_DEBUGGER_ENVIRONMENT "PATH=%PATH%;$<TARGET_FILE_DIR:Qt6::Core>;"
		WIN32_EXECUTABLE FALSE
		MACOSX_BUNDLE TRUE
)

# Installation
#if(WIN32)
#	get_target_property(_qmake_executable Qt5::qmake IMPORTED_LOCATION)
#	get_filename_component(_qt_bin_dir "${_qmake_executable}" DIRECTORY)
#	find_program(WINDEPLOYQT_EXECUTABLE windeployqt HINTS "${_qt_bin_dir}")
#	find_program(MACDEPLOYQT_EXECUTABLE macdeployqt HINTS "${_qt_bin_dir}")
#	add_custom_command(
#		TARGET ${PROJECT_NAME} POST_BUILD
#		COMMAND "${CMAKE_COMMAND}" -E env PATH="${_qt_bin_dir}" "${WINDEPLOYQT_EXECUTABLE}" "$<TARGET_FILE:${PROJECT_NAME}>"
#		COMMENT "Running windeployqt..."
#	)
#endif(WIN32)
