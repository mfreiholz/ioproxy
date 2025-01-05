qt_standard_project_setup()
set(name "echo-plugin")

set(headers
	EchoIO.hpp
	EchoPlugin.hpp
)

set(sources
	EchoIO.cpp
	EchoPlugin.cpp
)

add_library(${name} SHARED)
target_sources(${name} PRIVATE ${headers})
target_sources(${name} PRIVATE ${sources})
source_group(TREE ${CMAKE_CURRENT_SOURCE_DIR} PREFIX "Files" FILES ${headers} ${sources})
target_include_directories(${name} PRIVATE "${PROJECT_SOURCE_DIR}/src")
target_include_directories(${name} PRIVATE "${PROJECT_SOURCE_DIR}/src/ioproxy/include")
target_link_libraries(${name} PUBLIC Qt6::Core)
target_link_libraries(${name} PUBLIC tl-expected)
set_target_properties(${name} PROPERTIES OUTPUT_NAME "${name}.ioplugin" SUFFIX ".dll")
install(TARGETS ${name} DESTINATION "bin")

#temp fix
target_link_libraries(${name} PUBLIC ioproxy)