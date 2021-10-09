cmake_minimum_required(VERSION 3.20.0)

project(
	BalanceBot
)

include(Toolchains/avr-gcc.cmake)
avr_configure(atmega328p 12000000UL)

add_executable(${PROJECT_NAME})

avr_upload(${PROJECT_NAME} /dev/ttyACM0 19200)

target_include_directories(
	${PROJECT_NAME}
	PRIVATE
		include
)

target_sources(
	${PROJECT_NAME}
	PRIVATE
		src/main.c
		src/setup.c
		src/tasks.c
		src/stepper.c
		src/cmd.c
)

add_subdirectory(./lib/Serial)
target_link_libraries(${PROJECT_NAME} PRIVATE Serial)

add_subdirectory(./lib/I2C)
target_link_libraries(${PROJECT_NAME} PRIVATE I2C)

add_subdirectory(./lib/Timer)
target_link_libraries(${PROJECT_NAME} PRIVATE Timer)

add_subdirectory(./lib/GPIO)
target_link_libraries(${PROJECT_NAME} PRIVATE GPIO)

add_subdirectory(./lib/CLI)
target_link_libraries(${PROJECT_NAME} PRIVATE CLI)

add_subdirectory(./lib/TaskScheduler)
target_link_libraries(${PROJECT_NAME} PRIVATE TaskScheduler)

add_subdirectory(./lib/PID)
target_link_libraries(${PROJECT_NAME} PRIVATE PID)

add_subdirectory(./lib/IMU)
target_link_libraries(${PROJECT_NAME} PRIVATE MPU6050)
