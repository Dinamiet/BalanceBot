cmake_minimum_required(VERSION 3.20.0)

include(FetchContent)
set(FETCHCONTENT_QUIET OFF)

FetchContent_Declare(
	ATmega328P
	GIT_REPOSITORY
		git@bitbucket.org:dinamiet/atmega328p.git
	GIT_TAG
		development
	SOURCE_DIR
		${CMAKE_SOURCE_DIR}/lib/ATmega328P
)

FetchContent_MakeAvailable(
	ATmega328P
)
