include(FetchContent)

FetchContent_Declare(
	tl-expected
	GIT_REPOSITORY https://github.com/TartanLlama/expected.git
	GIT_TAG "v1.1.0"
	CONFIGURE_COMMAND ""
	BUILD_COMMAND ""
	#CMAKE_ARGS -DBuildShared=OFF -DBuildApps=OFF -DBuildTests=OFF
)

FetchContent_GetProperties(tl-expected)
if(NOT tl-expected_POPULATED)
  FetchContent_Populate(tl-expected)
endif()

add_library(tl-expected INTERFACE)
target_include_directories(tl-expected INTERFACE ${tl-expected_SOURCE_DIR}/include)

#FetchContent_MakeAvailable(tl-expected)