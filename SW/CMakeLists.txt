cmake_minimum_required(VERSION 3.20.0)

project(
	BalanceBot
)

include(Toolchains/avr-gcc.cmake)
avr_configure(atmega328p 12000000UL)

add_executable(${PROJECT_NAME})

avr_upload(${PROJECT_NAME} /dev/ttyACM0 19200)

target_compile_options(
    ${PROJECT_NAME}
    PRIVATE
        "-Wall" # Enable all common warnings
        "-Wextra" # Enable more warnings
        "-Wpedantic" # Warn about non-standard code
        "-Werror" # Warnings are treated as errors
        # "-Wshadow" # Warn about shadow variables
        "-Wdouble-promotion" # Warn when promoting floats to double
        "-Wformat=2" # printf and scanf warnings
        "-Wformat-security" # printf and scanf warnings
        "-Wformat-truncation" # printf and scanf warnings
        "-Wundef" # Warn when evaluating not defined flags
        "-fno-common" # Warn about multiple global variables with same name
        # "-Wconversion" # Warn about implicit conversions
        "-Wpadded" # Warn about padded structures
        "-flto" # Link time optimization
        "-fno-fat-lto-objects" # Link time optimization
        "-ffunction-sections" # Link time optimization help
        "-fdata-sections" # Link time optimization help
        "-Wno-unused-parameter" # Disable unused parameter warning - there are quite a few callbacks where the parameters are not used
)

target_link_options(
    ${PROJECT_NAME}
    PRIVATE
        "-Wl,--gc-sections" # Remove unwanted sections
        "-flto" # Link time optimization
        "-fno-fat-lto-objects" # Link time optimization
)


target_include_directories(
	${PROJECT_NAME}
	PRIVATE
		include
)

target_sources(
	${PROJECT_NAME}
	PRIVATE
		src/main.c
		src/cmdlist.c
		src/heartbeat.c
		src/imu.c
		src/motors.c
		src/steppers.c
		src/system.c
        src/control.c
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

add_subdirectory(./lib/Database)
target_link_libraries(${PROJECT_NAME} PRIVATE Database)

add_subdirectory(./lib/PID)
target_link_libraries(${PROJECT_NAME} PRIVATE PID)

add_subdirectory(./lib/IMU)
target_link_libraries(${PROJECT_NAME} PRIVATE MPU6050)
