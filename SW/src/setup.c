#include "setup.h"

#include "cli.h"
#include "cmd.h"
#include "gpio.h"
#include "i2c.h"
#include "mpu6050.h"
#include "serial.h"
#include "stepper.h"
#include "task_scheduler.h"
#include "tasks.h"
#include "timer.h"

#include <avr/interrupt.h>
#include <string.h>
#include <math.h>
#include <stdio.h>

/*---------------------------------------------------------------- */
/* Serial Interface setup */
/*---------------------------------------------------------------- */
#define SERIAL_BUFFER_SIZE 64
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
/* I2C setup */
/*---------------------------------------------------------------- */
#define I2C_TX_BUFFER_SIZE		   32
#define I2C_TRANSATION_BUFFER_SIZE 4

uint8_t		   TXBuff_i2c[I2C_TX_BUFFER_SIZE];
I2CTransaction TransBuff_i2c[I2C_TRANSATION_BUFFER_SIZE];

#define I2C_SPEED 100000 // Hz
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
/* Stepper Setup */
/*---------------------------------------------------------------- */
Steppers steppers;
void	 Setup_Steppers() { Steppers_Init(&steppers, GPIOD, GPIOC); }

/*---------------------------------------------------------------- */
/* GPIO setup */
/*---------------------------------------------------------------- */
void Setup_LED() { GPIO_PinMode(GPIOB, 5, GPIO_OUTPUT); }

/*---------------------------------------------------------------- */
/* CLI Interface setup */
/*---------------------------------------------------------------- */
#define CLI_CMD(cmd)                   \
	{                                  \
#cmd, Cmd_##cmd, CmdHelp_##cmd \
	}

CLI		   cli;
CLICommand cli_commands[] = {
		CLI_CMD(move),
		CLI_CMD(speed),
		CLI_CMD(hold),
		CLI_CMD(led),
		CLI_CMD(welcome),
		{"help", CLI_Cmd, CLI_Help},
		{0, 0, 0},
};

size_t cli_read(char* str, size_t max) { return Serial_Read(Serial0, str, max); }

size_t cli_write(char* str) { return Serial_Write(Serial0, str, strlen(str)); }

void Setup_CLI()
{
	CLI_Init(&cli, cli_commands, cli_read, cli_write);
	CLI_ProcessCommand(&cli, "welcome");
}

/*---------------------------------------------------------------- */
/* Task Scheduler interface setup */
/*---------------------------------------------------------------- */
#define MAX_TASKS 12
TaskList	taskList;
static Task taskBuffer[MAX_TASKS];

void Setup_TaskScheduler() { TaskScheduler_Init(&taskList, getSystemTime, taskBuffer, MAX_TASKS); }

/*---------------------------------------------------------------- */
/* IMU setup */
/*---------------------------------------------------------------- */
MPU6050 imu;

#define IMU_ADDRESS	   0x68
#define GYRO_OFFSET_X  -45
#define GYRO_OFFSET_Y  -54
#define GYRO_OFFSET_Z  28
#define ACCEL_OFFSET_X -6999
#define ACCEL_OFFSET_Y 4933
#define ACCEL_OFFSET_Z 8439

Task*		   IMU_Available;
Task*		   IMU_Interrupt;
Task*		   IMU_PacketReady;
static uint8_t availablePackets;

static void imu_packetReady(void* _) { TaskScheduler_ActivateTask(IMU_PacketReady); }

static void imu_available(void* _) { TaskScheduler_ActivateTask(IMU_Available); }

static void imu_Interrupt(void* _) { TaskScheduler_ActivateTask(IMU_Interrupt); }

static void imu_HandleInterrupt(void* imu)
{
	MPU6050_RequestAvailablePackets(imu, imu_available);

	TaskScheduler_DeactivateTask(IMU_Interrupt);
}

static void imu_HandleAvailable(void* imu)
{
	availablePackets = MPU6050_AvailablePackets(imu);
	if (availablePackets)
		MPU6050_RequestPacket(imu, imu_packetReady);

	TaskScheduler_DeactivateTask(IMU_Available);
}

static void imu_HandlePacket(void* imu)
{
	MPU6050_YPR ypr = MPU6050_YawPitchRoll(imu);
	if (availablePackets--)
		MPU6050_RequestPacket(imu, imu_packetReady);

	TaskScheduler_DeactivateTask(IMU_PacketReady);

	char buff[64];
	sprintf(buff, "%.2f\t%.2f\t%.2f\n", ypr.Yaw * 180.0f / M_PI, ypr.Pitch * 180.0f / M_PI, ypr.Roll * 180.0f / M_PI);
	Serial_Write(Serial0, buff, strlen(buff));
}

static void imu_Configure(void* imu)
{
	char* buff= "IMU Configure";
	Serial_Write(Serial0, buff, strlen(buff));

	MPU6050_Configure(imu);

	MPU6050_Offset offset;

	offset.X = ACCEL_OFFSET_X;
	offset.Y = ACCEL_OFFSET_Y;
	offset.Z = ACCEL_OFFSET_Z;
	MPU6050_SetAccelOffset(imu, offset);

	offset.X = GYRO_OFFSET_X;
	offset.Y = GYRO_OFFSET_Y;
	offset.Z = GYRO_OFFSET_Z;
	MPU6050_SetGyroOffset(imu, offset);

	GPIO_PinMode(GPIOD, 2, GPIO_INPUT);
	GPIO_EnableInterrupt(EXTERNAL_INTERRUPT_0, INTERRUPT_TRIGGER_RISING, imu_Interrupt);
}

void Setup_IMU()
{
	MPU6050_Init(&imu, I2C0, IMU_ADDRESS);

	TaskScheduler_CreateSingleShotTask(&taskList, "IMU Configure", imu_Configure, &imu, 100);

	IMU_Interrupt	= TaskScheduler_CreateSingleShotTask(&taskList, "IMU Interrupt Handler", imu_HandleInterrupt, &imu, 0);
	IMU_Available	= TaskScheduler_CreateSingleShotTask(&taskList, "IMU Available Handler", imu_HandleAvailable, &imu, 0);
	IMU_PacketReady = TaskScheduler_CreateSingleShotTask(&taskList, "IMU Packet Handler", imu_HandlePacket, &imu, 0);

	TaskScheduler_DeactivateTask(IMU_Interrupt);
	TaskScheduler_DeactivateTask(IMU_Available);
	TaskScheduler_DeactivateTask(IMU_PacketReady);
}

/*---------------------------------------------------------------- */
/* Tasks */
/*---------------------------------------------------------------- */
void Create_Tasks()
{
	TaskScheduler_CreateRetriggerTask(&taskList, CLI_TASK_NAME, Task_CLI, &cli, CLI_PERIOD);
	TaskScheduler_CreateRetriggerTask(&taskList, LED_TASK_NAME, Task_LED, NULL, LED_PERIOD);
	TaskScheduler_CreateRetriggerTask(&taskList, STEPPER_TASK_NAME, Task_Stepper, &steppers, STEP_SPEED);
	TaskScheduler_CreateRetriggerTask(&taskList, STEPPER_HOLD_TASK_NAME, Task_StepperDisable, &steppers, STEPPER_HOLD / 2);
}
