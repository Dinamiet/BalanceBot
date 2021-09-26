cmake_minimum_required(VERSION 3.20.0)

project(
	BalanceBot
)

include(Toolchains/avr-gcc.cmake)
avr_configure(atmega328p 16000000UL)

add_executable(${PROJECT_NAME})

avr_upload(${PROJECT_NAME} /dev/ttyACM0 115200)

target_sources(
	${PROJECT_NAME}
	PRIVATE
		main.c
)
