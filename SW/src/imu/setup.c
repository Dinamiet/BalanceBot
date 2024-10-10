#include "drivers.h"
#include "gpio.h"
#include "imu.h"
#include "tasks.h"
#include "utilities.h"

#include <avr/pgmspace.h>
#include <math.h>

#define IMU_DEVICE_ADDRESS 0x68

#define GYRO_OFFSET_X -52
#define GYRO_OFFSET_Y -68
#define GYRO_OFFSET_Z 33

#define ACCEL_OFFSET_X -6899
#define ACCEL_OFFSET_Y 4904
#define ACCEL_OFFSET_Z 8433

#define RAD_TO_DEG(x) (x * 180.0f / (float)M_PI)

MPU* imu;

static MPU           mpu6050;
static SchedulerTask imuConfigTask;
static SchedulerTask imuPacketProcessing;
static const uint8_t dmpFirmware[] PROGMEM = {
#include "mpu6050_dmpfw.txt"
};

static size_t imuRead(void* data, const size_t size);
static size_t imuWrite(const void* data, const size_t size);
static bool   imuRequest(const MPU* mpu, const size_t size, const MPU_Complete complete);
static size_t mpuDMPFirmwareRead(void* data, const size_t offset, size_t size);
static void   imuConfigure(MPU* mpu);
static void   imuDataReady();
static void   imuAvailablePacketHandler(const bool success, const MPU* mpu, const size_t size);
static void   imuPacketDataReady(const bool success, const MPU* mpu, const size_t size);
static void   imuProcess(void* data);

static size_t imuRead(void* data, const size_t size) { return I2C_Read(i2c, data, size); }

static size_t imuWrite(const void* data, const size_t size) { return I2C_Write(i2c, IMU_DEVICE_ADDRESS, data, size, NULL, NULL); }

static bool imuRequest(const MPU* mpu, const size_t size, const MPU_Complete complete)
{
	return I2C_Request(i2c, IMU_DEVICE_ADDRESS, size, (I2C_Complete)complete, mpu);
}

static size_t mpuDMPFirmwareRead(void* data, const size_t offset, size_t size)
{
	memcpy_P(data, &dmpFirmware[offset], size);
	return size;
}

static void imuConfigure(MPU* mpu)
{
	MPU_Configure(mpu, mpuDMPFirmwareRead);
	MPUOffset offsets;

	offsets.X = GYRO_OFFSET_X;
	offsets.Y = GYRO_OFFSET_Y;
	offsets.Z = GYRO_OFFSET_Z;
	MPU_SetGyroOffset(imu, offsets);

	offsets.X = ACCEL_OFFSET_X;
	offsets.Y = ACCEL_OFFSET_Y;
	offsets.Z = ACCEL_OFFSET_Z;
	MPU_SetAccelOffset(imu, offsets);

	GPIO_EnableIRQ(GPIO_ISR0, GPIO_INTERRUPT_TRIGGER_RISING, imuDataReady);

	MPU_Enable(imu);
}

static void imuDataReady() { MPU_RequestAvailablePackets(imu, imuAvailablePacketHandler); }

static void imuAvailablePacketHandler(const bool success, const MPU* mpu, const size_t size)
{
	(void)size;
	if (success)
	{
		uint16_t numPackets = MPU_AvailablePackets(mpu);
		if (numPackets)
			MPU_RequestPacket(mpu, imuPacketDataReady);
	}
}

static void imuPacketDataReady(const bool success, const MPU* mpu, const size_t size)
{
	(void)size;
	(void)mpu;
	if (success)
		Scheduler_Activate(&imuPacketProcessing);
}

static void imuProcess(void* data)
{
	(void)data;

	Quaternion q = MPU_PacketQuaternion(imu);
	float      y, p, r;
	Quaternion_YawPitchRoll(q, &y, &p, &r);
	CLI_Write(cmdLine, "%d %d %d\n\r", (int)RAD_TO_DEG(y), (int)RAD_TO_DEG(p), (int)RAD_TO_DEG(r));
}

void Setup_IMU()
{
	imu = &mpu6050;
	MPU_Init(imu, imuRead, imuWrite, imuRequest);

	Scheduler_CreateSingleTask(taskScheduler, &imuConfigTask, IMU_CONFIG_TASK, (Scheduler_TaskFunction)imuConfigure, imu, IMU_TASK_DELAY);
	Scheduler_CreateRecurringTask(taskScheduler, &imuPacketProcessing, IMU_PACKET_PROCESSING_TASK, imuProcess, NULL, 0);
	Scheduler_Deactivate(&imuPacketProcessing);
}
