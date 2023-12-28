# Qt Installation Path
if(WIN32)
	list(APPEND CMAKE_PREFIX_PATH "C:/Qt/6.6.1/msvc2019_64")
endif()

find_package(Qt6 REQUIRED
	COMPONENTS
		Core
		Network
		Concurrent
		Gui
		Widgets
		Quick
		Qml
		Xml
		WebSockets
		SerialPort
)

qt_standard_project_setup()