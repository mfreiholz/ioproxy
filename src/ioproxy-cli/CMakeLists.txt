qt_standard_project_setup()
set(name "ioproxy-cli")
set(src_root "${CMAKE_CURRENT_SOURCE_DIR}/src")

set(files
	"${src_root}/CmdLineConfig.cpp"
	"${src_root}/CmdLineConfig.hpp"

	"${src_root}/Config.cpp"
	"${src_root}/Config.hpp"
	
	"${src_root}/ConsoleStatusPrinter.cpp"
	"${src_root}/ConsoleStatusPrinter.hpp"

	"${src_root}/IOProxyConsoleApplication.cpp"
	"${src_root}/IOProxyConsoleApplication.hpp"
	
	"${src_root}/Main.cpp"
)

add_executable(${name})
target_sources(${name} PRIVATE ${files})
source_group(TREE ${src_root} PREFIX "SOURCES" FILES ${files})
target_link_libraries(${name} PRIVATE ioproxy)
install(TARGETS ${name} DESTINATION "bin")