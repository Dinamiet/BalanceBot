cmake_minimum_required(VERSION 3.20.0)

include(FetchContent)
set(FETCHCONTENT_QUIET OFF)

FetchContent_Declare(
	AVR
	GIT_REPOSITORY
		git@github.com:Dinamiet/AVR.git
	GIT_TAG
		main
	SOURCE_DIR
		${CMAKE_SOURCE_DIR}/lib/AVR
)

FetchContent_Declare(
	Scheduler
	GIT_REPOSITORY
		git@github.com:Dinamiet/Scheduler.git
	GIT_TAG
		main
	SOURCE_DIR
		${CMAKE_SOURCE_DIR}/lib/Scheduler
)

FetchContent_Declare(
	MPU6050
	GIT_REPOSITORY
		git@github.com:Dinamiet/MPU6050.git
	GIT_TAG
		main
	SOURCE_DIR
		${CMAKE_SOURCE_DIR}/lib/MPU6050
)

FetchContent_Declare(
	Observer
	GIT_REPOSITORY
		git@github.com:Dinamiet/Observer.git
	GIT_TAG
		main
	SOURCE_DIR
		${CMAKE_SOURCE_DIR}/lib/Observer
)

FetchContent_Declare(
	PID
	GIT_REPOSITORY
		git@github.com:Dinamiet/PID.git
	GIT_TAG
		main
	SOURCE_DIR
		${CMAKE_SOURCE_DIR}/lib/PID
)

FetchContent_Declare(
	Stepper
	GIT_REPOSITORY
		git@github.com:Dinamiet/Stepper.git
	GIT_TAG
		main
	SOURCE_DIR
		${CMAKE_SOURCE_DIR}/lib/Stepper
)

FetchContent_Declare(
	DataPacket
	GIT_REPOSITORY
		git@github.com:Dinamiet/DataPacket.git
	GIT_TAG
		main
	SOURCE_DIR
		${CMAKE_SOURCE_DIR}/lib/DataPacket
)

FetchContent_MakeAvailable(
	AVR
	Scheduler
	MPU6050
	Observer
	PID
	Stepper
	DataPacket
)
