set(name "ioproxy-gui-widgets")
qt_standard_project_setup()

qt_add_executable(
	${name}
	Main.cpp

	Widgets/IOProxyMainWindow.cpp
	Widgets/IOProxyMainWindow.hpp
	Widgets/IOProxyMainWindow.ui

)

#qt_add_resources(
#	${name} "controls"
#	PREFIX
#		"/"
#	FILES
#		"qtquickcontrols2.conf"
#)

target_link_libraries(
	${name}
	PRIVATE
		ioproxy
		Qt6::Core
		Qt6::Gui
		Qt6::Widgets
		Qt6::Xml
		humblelogging
)

set_target_properties(
	${name} PROPERTIES
		VS_DEBUGGER_ENVIRONMENT "PATH=%PATH%;$<TARGET_FILE_DIR:Qt6::Core>;"
		WIN32_EXECUTABLE FALSE
		MACOSX_BUNDLE TRUE
)