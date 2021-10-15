#include "gpio.h"
#include "mpu6050.h"
#include "serial.h"
#include "setup.h"
#include "task_scheduler.h"

#include <math.h>
#include <stdio.h>
#include <string.h>

#define PI (float)M_PI

extern TaskList taskList;

MPU6050 imu;

Task* IMU_Available;
Task* IMU_Interrupt;
Task* IMU_PacketReady;

static uint8_t availablePackets;

static void imu_packetReady(void* _) { TaskScheduler_ActivateTask(IMU_PacketReady); }

static void imu_available(void* _) { TaskScheduler_ActivateTask(IMU_Available); }

static void imu_Interrupt(void* _) { TaskScheduler_ActivateTask(IMU_Interrupt); }

static void imu_HandleInterrupt(void* imu)
{
	MPU6050_RequestAvailablePackets(imu, imu_available);

	TaskScheduler_DeactivateTask(IMU_Interrupt);

	char* buff = "HandleInterrupt\n";
	Serial_Write(Serial0, buff, strlen(buff));
}

static void imu_HandleAvailable(void* imu)
{
	availablePackets = MPU6050_AvailablePackets(imu);
	if (availablePackets)
		MPU6050_RequestPacket(imu, imu_packetReady);

	TaskScheduler_DeactivateTask(IMU_Available);

	char buff[64];
	sprintf(buff, "HandleAvailable: %d\n", availablePackets);
	Serial_Write(Serial0, buff, strlen(buff));
}

static void imu_HandlePacket(void* imu)
{
	MPU6050_YPR ypr = MPU6050_YawPitchRoll(imu);
	if (availablePackets--)
		MPU6050_RequestPacket(imu, imu_packetReady);

	TaskScheduler_DeactivateTask(IMU_PacketReady);

	char buff[64];
	sprintf(buff, "%.2f\t%.2f\t%.2f\n", (double)(ypr.Yaw * 180.0f / PI), (double)(ypr.Pitch * 180.0f / PI), (double)(ypr.Roll * 180.0f / PI));
	Serial_Write(Serial0, buff, strlen(buff));
}

void calibrationProgress(char c) { Serial_Write(Serial0, &c, 1); }

#include <util/delay.h>

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

	IMU_Interrupt	= TaskScheduler_CreateSingleShotTask(&taskList, "IMU Interrupt Handler", imu_HandleInterrupt, &imu, 0);
	IMU_Available	= TaskScheduler_CreateSingleShotTask(&taskList, "IMU Available Handler", imu_HandleAvailable, &imu, 0);
	IMU_PacketReady = TaskScheduler_CreateSingleShotTask(&taskList, "IMU Packet Handler", imu_HandlePacket, &imu, 0);

	TaskScheduler_DeactivateTask(IMU_Interrupt);
	TaskScheduler_DeactivateTask(IMU_Available);
	TaskScheduler_DeactivateTask(IMU_PacketReady);

	TaskScheduler_CreateSingleShotTask(&taskList, "IMU Configure", imu_Configure, &imu, 100);
}

void Cmd_imu_req(CLI* cli, int argc, char* argv[]) { imu_Interrupt(NULL); }

char* CmdHelp_imu_req[] = {
		"Issue Available Packet Request to IMU",
		"Usage: imu_req",
		0,
};

void Cmd_cal_gyro(CLI* cli, int argc, char* argv[])
{
	MPU6050_CalibrateGyro(&imu, 100, 3, calibrationProgress);
	MPU6050_RequestGyroOffset(&imu, NULL);
	while (I2C_IsActive(imu.I2c)) {};

	char buff[64];

	MPU6050_Offset gyroOffset = MPU6050_GyroOffset(&imu);

	sprintf(buff, "\nGyro: %d\t%d\t%d\n", gyroOffset.X, gyroOffset.Y, gyroOffset.Z);
	cli->Write(buff);
}

char* CmdHelp_cal_gyro[] = {
		"Calibrate Gyro",
		"Usage: cal_gyro",
		0,
};

void Cmd_cal_accel(CLI* cli, int argc, char* argv[])
{
	MPU6050_CalibrateAccel(&imu, 100, 25, 2, calibrationProgress);
	MPU6050_RequestAccelOffset(&imu, NULL);
	while (I2C_IsActive(imu.I2c)) {};

	char buff[64];

	MPU6050_Offset accelOffset = MPU6050_AccelOffset(&imu);

	sprintf(buff, "Accel: %d\t%d\t%d\n", accelOffset.X, accelOffset.Y, accelOffset.Z);
	Serial_Write(Serial0, buff, strlen(buff));
}

char* CmdHelp_cal_accel[] = {
		"Calibrate Accel",
		"Usage: cal_accel",
		0,
};
