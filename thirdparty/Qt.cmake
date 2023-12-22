# Qt Installation Path
# Allows to define a custom environment variable "IOPROXY_QTDIR" to override the default "QTDIR" variable.
#if(WIN32)
#	list(APPEND CMAKE_PREFIX_PATH "C:/Qt/6.6.0/msvc2019_64")
#endif()

find_package(Qt6 REQUIRED COMPONENTS Core Network SerialPort WebSockets Concurrent Gui Widgets Quick Qml Xml)
