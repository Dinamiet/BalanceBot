cmake_minimum_required(VERSION 3.20)

include(FetchContent)
set(FETCHCONTENT_QUIET OFF)

FetchContent_Declare(
	DataPacket
	GIT_REPOSITORY
		git@bitbucket.org:dinamiet/datapacket.git
	GIT_TAG
		development
	SOURCE_DIR
		${CMAKE_SOURCE_DIR}/lib/DataPacket
)

FetchContent_MakeAvailable(
	DataPacket
)
