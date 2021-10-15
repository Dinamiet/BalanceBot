#include "system.h"

#include "i2c.h"
#include "serial.h"
#include "setup.h"
#include "task_scheduler.h"
#include "timer.h"

/*---------------------------------------------------------------- */
/* Serial Interface setup */
/*---------------------------------------------------------------- */
static uint8_t TXBuff_serial[SERIAL_BUFFER_SIZE];
static uint8_t RXBuff_serial[SERIAL_BUFFER_SIZE / 4];

void Setup_Serial()
{
	Serial_Configure(Serial0, SERIAL_8N1, SERIAL_BAUD);
	Serial_InitTXBuffer(Serial0, TXBuff_serial, sizeof(TXBuff_serial));
	Serial_InitRXBuffer(Serial0, RXBuff_serial, sizeof(RXBuff_serial));
	Serial_Enable(Serial0);
}

/*---------------------------------------------------------------- */
/* I2C setup */
/*---------------------------------------------------------------- */
static uint8_t		  TXBuff_i2c[I2C_TX_BUFFER_SIZE];
static I2CTransaction TransBuff_i2c[I2C_TRANSATION_BUFFER_SIZE];

void Setup_I2C()
{
	I2C_Configure(I2C0, I2C_SPEED);
	I2C_InitTXBuffer(I2C0, TXBuff_i2c, sizeof(TXBuff_i2c));
	I2C_InitTransBuffer(I2C0, TransBuff_i2c, sizeof(TransBuff_i2c));
	I2C_Enable(I2C0);
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
	float ms = TIME_ROLLOVER_CONVERSION * rollOverCount;
	ms += TIMER_COUNT_TO_MS * Timer_GetValue(Timer2);
	return ms;
}

void Setup_SystemTime()
{
	Timer_Configure(Timer2, TIMER_DIV_1024);
	Timer_EnableInterrupt(Timer2, TIMER_ROLLOVER_INT, TimerRollover);
	Timer_Enable(Timer2, TIMER_NORMAL);
}

/*---------------------------------------------------------------- */
/* Task Scheduler interface setup */
/*---------------------------------------------------------------- */
TaskList	taskList;
static Task taskBuffer[MAX_TASKS];

void Setup_TaskScheduler() { TaskScheduler_Init(&taskList, getSystemTime, taskBuffer, MAX_TASKS); }
