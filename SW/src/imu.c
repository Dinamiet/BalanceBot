#include "database.h"
#include "gpio.h"
#include "mpu6050.h"
#include "serial.h"
#include "setup.h"
#include "task_scheduler.h"

#include <stdio.h>

extern TaskList taskList;
extern Database db;

MPU6050 imu;

static volatile bool	packetReady;
static volatile bool	availableReady;
static volatile bool	interruptReady;
static volatile uint8_t availablePackets;

static void imu_packetReady(void* _) { packetReady = true; }

static void imu_available(void* _) { availableReady = true; }

static void imu_Interrupt(void* _) { interruptReady = true; }

static void imu_HandleInterrupt(void* imu)
{
	if (interruptReady)
	{
		interruptReady = false;
		MPU6050_RequestAvailablePackets(imu, imu_available);
	}
}

static void imu_HandleAvailable(void* imu)
{
	if (availableReady)
	{
		availableReady	 = false;
		availablePackets = MPU6050_AvailablePackets(imu);
		if (availablePackets)
			MPU6050_RequestPacket(imu, imu_packetReady);
	}
}

static void imu_HandlePacket(void* imu)
{
	if (packetReady)
	{
		packetReady		= false;
		MPU6050_YPR ypr = MPU6050_YawPitchRoll(imu);

		Database_Notify(&db, IMU_DATA_NOTIFY, &ypr);
	}
}

void calibrationProgress(char c) { Serial_Write(Serial0, &c, 1); }

static void imu_Configure(void* imu)
{
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

	MPU6050_Enable(imu);
}

void Setup_IMU()
{
	MPU6050_Init(&imu, I2C0, IMU_ADDRESS);

	TaskScheduler_CreateRetriggerTask(&taskList, NULL, imu_HandleInterrupt, &imu, 0);
	TaskScheduler_CreateRetriggerTask(&taskList, NULL, imu_HandleAvailable, &imu, 0);
	TaskScheduler_CreateRetriggerTask(&taskList, NULL, imu_HandlePacket, &imu, 0);

	TaskScheduler_CreateSingleShotTask(&taskList, NULL, imu_Configure, &imu, 100);
}

void Cmd_imu_avail(CLI* cli, int argc, char* argv[])
{
	char buff[5];
	sprintf(buff, "%d\n", availablePackets);
	cli->Write("Available: ");
	cli->Write(buff);
}

char* CmdHelp_imu_avail[] = {
		// "Issue Available Packet Request to IMU",
		// "Usage: imu_req",
		0,
};

void Cmd_cal_gyro(CLI* cli, int argc, char* argv[])
{
	MPU6050_Disable(&imu);
	MPU6050_CalibrateGyro(&imu, 100, 3, calibrationProgress);
	MPU6050_RequestGyroOffset(&imu, NULL);
	while (I2C_IsActive(imu.I2c)) {};

	char buff[64];

	MPU6050_Offset gyroOffset = MPU6050_GyroOffset(&imu);

	sprintf(buff, "\nGyro: %d\t%d\t%d\n", gyroOffset.X, gyroOffset.Y, gyroOffset.Z);
	cli->Write(buff);

	MPU6050_Enable(&imu);
}

char* CmdHelp_cal_gyro[] = {
		// "Calibrate Gyro",
		// "Usage: cal_gyro",
		0,
};

void Cmd_cal_accel(CLI* cli, int argc, char* argv[])
{
	MPU6050_Disable(&imu);
	MPU6050_CalibrateAccel(&imu, 100, 25, 2, calibrationProgress);
	MPU6050_RequestAccelOffset(&imu, NULL);
	while (I2C_IsActive(imu.I2c)) {};

	char buff[64];

	MPU6050_Offset accelOffset = MPU6050_AccelOffset(&imu);

	sprintf(buff, "\nAccel: %d\t%d\t%d\n", accelOffset.X, accelOffset.Y, accelOffset.Z);
	cli->Write(buff);

	MPU6050_Enable(&imu);
}

char* CmdHelp_cal_accel[] = {
		// "Calibrate Accel",
		// "Usage: cal_accel",
		0,
};
