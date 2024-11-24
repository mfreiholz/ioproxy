set(name "ioproxy-cli")
qt_standard_project_setup()

set(headers
	src/Config.hpp
	src/CmdLineConfig.hpp
	src/ConsoleStatusPrinter.hpp
	src/IOProxyConsoleApplication.hpp
)

set(sources
	src/Config.cpp
	src/CmdLineConfig.cpp
	src/ConsoleStatusPrinter.cpp
	src/IOProxyConsoleApplication.cpp
	src/Main.cpp
)

add_executable(${name})
target_sources(${name} PRIVATE ${headers})
target_sources(${name} PRIVATE ${sources})
source_group(TREE ${CMAKE_CURRENT_SOURCE_DIR} PREFIX "Files" FILES ${headers} ${sources})

target_link_libraries(
	${name}
	PRIVATE
		Qt6::Core
		humblelogging
		ioproxy
)

install(
	TARGETS ${name}
	DESTINATION "bin"
)

#if(WIN32)
#	include(Windeployqt)
#	windeployqt(${name} ${INSTALL_BIN_PATH})
#endif()