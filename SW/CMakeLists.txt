cmake_minimum_required(VERSION 3.20.0)

project(
	BalanceBot
)

include(Toolchains/avr-gcc.cmake)
avr_configure(atmega328p 16000000UL)

add_executable(${PROJECT_NAME})

avr_upload(${PROJECT_NAME} /dev/ttyACM0 115200)

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
)

add_subdirectory(./lib/Serial)
target_link_libraries(${PROJECT_NAME} PRIVATE Serial)

add_subdirectory(./lib/CLI)
target_link_libraries(${PROJECT_NAME} PRIVATE CLI)
