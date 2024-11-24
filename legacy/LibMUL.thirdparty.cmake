include(ExternalProject)

ExternalProject_Add(
	LibMUL

	GIT_REPOSITORY https://github.com/mfreiholz/libmul-cpp.git
	GIT_TAG "main"

	CONFIGURE_COMMAND ""
	BUILD_COMMAND ""
	INSTALL_COMMAND ""
	TEST_COMMAND ""
)
ExternalProject_Get_Property(LibMUL SOURCE_DIR)
message("LibMUL SOURCE_DIR = ${SOURCE_DIR}")

set(LibMUL_INCLUDE_DIRS "${SOURCE_DIR}")
include_directories(${LibMUL_INCLUDE_DIRS})