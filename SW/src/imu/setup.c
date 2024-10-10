#include "drivers.h"
#include "imu.h"
#include "tasks.h"
#include "utilities.h"

#include <avr/pgmspace.h>

#define IMU_DEVICE_ADDRESS 0x68

#define GYRO_OFFSET_X -45
#define GYRO_OFFSET_Y -54
#define GYRO_OFFSET_Z 28

#define ACCEL_OFFSET_X -6982
#define ACCEL_OFFSET_Y 4885
#define ACCEL_OFFSET_Z 8439

MPU* imu;

static MPU           mpu6050;
static SchedulerTask imuConfigTask;
static const uint8_t dmpFirmware[] PROGMEM = {
#include "mpu6050_dmpfw.txt"
};

static size_t imuRead(void* data, const size_t size);
static size_t imuWrite(const void* data, const size_t size);
static bool   imuRequest(const MPU* mpu, const size_t size, const MPU_Complete complete);
static size_t mpuDMPFirmwareRead(void* data, const size_t offset, size_t size);
static void   imuConfigure(MPU* mpu);

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
}

void Setup_IMU()
{
	imu = &mpu6050;
	MPU_Init(imu, imuRead, imuWrite, imuRequest);

	Scheduler_CreateSingleTask(taskScheduler, &imuConfigTask, IMU_CONFIG_TASK, (Scheduler_TaskFunction)imuConfigure, imu, IMU_TASK_DELAY);
}
