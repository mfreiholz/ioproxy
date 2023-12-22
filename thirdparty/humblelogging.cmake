include(FetchContent)

FetchContent_Declare(
	humblelogging
	GIT_REPOSITORY https://github.com/mfreiholz/humblelogging.git
	GIT_TAG "main"
	CMAKE_ARGS -DBuildShared=OFF -DBuildApps=OFF -DBuildTests=OFF
)

FetchContent_MakeAvailable(humblelogging)