cmake_minimum_required(VERSION 3.20)

include(FetchContent)
set(FETCHCONTENT_QUIET OFF)

FetchContent_Declare(
	DataPacket
	GIT_REPOSITORY
		git@github.com:Dinamiet/DataPacket.git
	GIT_TAG
		main
	SOURCE_DIR
		${CMAKE_SOURCE_DIR}/lib/DataPacket
)

FetchContent_Declare(
	CLI
	GIT_REPOSITORY
		git@github.com:Dinamiet/CLI.git
	GIT_TAG
		main
	SOURCE_DIR
		${CMAKE_SOURCE_DIR}/lib/CLI
)


FetchContent_MakeAvailable(
	DataPacket
	CLI
)
