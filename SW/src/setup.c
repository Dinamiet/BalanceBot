#include "setup.h"

#include "cli.h"
#include "gpio.h"
#include "serial.h"
#include "task_scheduler.h"
#include "tasks.h"
#include "timer.h"

#include <avr/interrupt.h>

#include <string.h>

/*---------------------------------------------------------------- */
/* Serial Interface setup */
/*---------------------------------------------------------------- */
#define SERIAL_BUFFER_SIZE 128
uint8_t TXBuff_serial[SERIAL_BUFFER_SIZE];
uint8_t RXBuff_serial[SERIAL_BUFFER_SIZE / 4];

#define SERIAL_BAUD 115200
void Setup_Serial()
{
	Serial_Configure(Serial0, SERIAL_8N1, SERIAL_BAUD);
	Serial_InitTXBuffer(Serial0, TXBuff_serial, sizeof(TXBuff_serial));
	Serial_InitRXBuffer(Serial0, RXBuff_serial, sizeof(RXBuff_serial));
	Serial_Enable(Serial0);
}

/*---------------------------------------------------------------- */
/* Timer setup */
/*---------------------------------------------------------------- */
#define TIMER_COUNT_TO_MS		 ((1024.0f * 1000.0f) / F_CPU)
#define TIME_ROLLOVER_CONVERSION (TIMER_COUNT_TO_MS * 256.0f)
static uint32_t rollOverCount = 0;
static void		TimerRollover() { rollOverCount += 1; }

uint32_t getSystemTime()
{
	cli();
	uint32_t cnt	   = rollOverCount;
	uint8_t	 remainder = Timer_GetValue(Timer2);
	sei();

	float ms = TIME_ROLLOVER_CONVERSION * cnt;
	ms += remainder * TIMER_COUNT_TO_MS;

	return ms;
}

void Setup_SystemTime()
{
	Timer_Configure(Timer2, TIMER_DIV_1024);
	Timer_EnableInterrupt(Timer2, TIMER_ROLLOVER_INT, TimerRollover);
	Timer_Enable(Timer2, TIMER_NORMAL);
}

/*---------------------------------------------------------------- */
/* GPIO setup */
/*---------------------------------------------------------------- */
void Setup_GPIO() { GPIO_PinMode(GPIOB, 5, GPIO_OUTPUT); }

/*---------------------------------------------------------------- */
/* CLI Interface setup */
/*---------------------------------------------------------------- */
CLI		   cli;
CLICommand cli_commands[] = {{"help", CLI_Cmd, CLI_Help}, {0, 0, 0}};

size_t cli_read(char* str, size_t max) { return Serial_Read(Serial0, str, max); }

size_t cli_write(char* str) { return Serial_Write(Serial0, str, strlen(str)); }

void Setup_CLI()
{
	CLI_Init(&cli, cli_commands, cli_read, cli_write);
	CLI_ProcessCommand(&cli, "help");
}

/*---------------------------------------------------------------- */
/* Task Scheduler interface setup */
/*---------------------------------------------------------------- */
#define MAX_TASKS 4
TaskList	taskList;
static Task taskBuffer[MAX_TASKS];

void Setup_TaskScheduler() { TaskScheduler_Init(&taskList, getSystemTime, taskBuffer, MAX_TASKS); }

/*---------------------------------------------------------------- */
/* Tasks */
/*---------------------------------------------------------------- */
void Create_Tasks()
{
	TaskScheduler_CreateRetriggerTask(&taskList, "CLI", Task_CLI, &cli, CLI_PERIOD);
	TaskScheduler_CreateRetriggerTask(&taskList, "LED", Task_LED, NULL, LED_PERIOD);
}
