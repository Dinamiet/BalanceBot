cmake_minimum_required(VERSION 3.20.0)

include(FetchContent)
set(FETCHCONTENT_QUIET OFF)

FetchContent_Declare(
	AVR
	GIT_REPOSITORY
		git@bitbucket.org:dinamiet/avr.git
	GIT_TAG
		development
	SOURCE_DIR
		${CMAKE_SOURCE_DIR}/lib/AVR
)

FetchContent_Declare(
	Scheduler
	GIT_REPOSITORY
		git@bitbucket.org:dinamiet/scheduler.git
	GIT_TAG
		development
	SOURCE_DIR
		${CMAKE_SOURCE_DIR}/lib/Scheduler
)

FetchContent_Declare(
	CLI
	GIT_REPOSITORY
		git@bitbucket.org:dinamiet/cli.git
	GIT_TAG
		development
	SOURCE_DIR
		${CMAKE_SOURCE_DIR}/lib/CLI
)

FetchContent_Declare(
	MPU6050
	GIT_REPOSITORY
		git@bitbucket.org:dinamiet/mpu6050.git
	GIT_TAG
		development
	SOURCE_DIR
		${CMAKE_SOURCE_DIR}/lib/MPU6050
)

FetchContent_Declare(
	Observer
	GIT_REPOSITORY
		git@bitbucket.org:dinamiet/observer.git
	GIT_TAG
		development
	SOURCE_DIR
		${CMAKE_SOURCE_DIR}/lib/Observer
)


FetchContent_MakeAvailable(
	AVR
	Scheduler
	CLI
	MPU6050
)
