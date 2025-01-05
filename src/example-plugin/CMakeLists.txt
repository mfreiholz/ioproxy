qt_standard_project_setup()
set(name "example-plugin")

set(headers
	EchoIO.hpp
	ExamplePlugin.hpp
)

set(sources
	EchoIO.cpp
	ExamplePlugin.cpp
)

add_library(${name} SHARED)
target_sources(${name} PRIVATE ${headers})
target_sources(${name} PRIVATE ${sources})
source_group(TREE ${CMAKE_CURRENT_SOURCE_DIR} PREFIX "Files" FILES ${headers} ${sources})
target_include_directories(${name} PRIVATE "${CMAKE_CURRENT_SOURCE_DIR}/../ioproxy/include")

target_link_libraries(
	${name}
	PUBLIC
		ioproxy
)