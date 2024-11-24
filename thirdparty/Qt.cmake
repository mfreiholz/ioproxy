# Qt Installation Path
#if(WIN32)
#	list(APPEND CMAKE_PREFIX_PATH "C:/Qt/6.6.1/msvc2019_64")
#endif()

find_package(
	Qt6
	REQUIRED
	COMPONENTS
		Core
		Network
		Concurrent
		WebSockets
		SerialPort
		#Gui
		#Widgets
		#Quick
		#Qml
		#Xml
	PATHS
		"D:/Development/C_Workspace/thirdparty/qt/6.8.0/msvc2022_64"
)

set(
	CMAKE_VS_DEBUGGER_ENVIRONMENT
	"PATH=$<TARGET_FILE_DIR:Qt6::Core>;%PATH%"
)

qt_standard_project_setup()