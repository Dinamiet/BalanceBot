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
I2CDevice imuDevice;

static MPU           mpu6050;
static SchedulerTask imuConfigTask;
static SchedulerTask imuInterruptTask;
static SchedulerTask imuPacketAvailableTask;
static SchedulerTask imuPacketDataReadyTask;

static volatile bool imuInterruptTriggered;
static volatile bool imuPacketAvailableTriggered;
static volatile bool imuPacketDataReadyTriggered;

static const uint8_t dmpFirmware[] PROGMEM = {
#include "mpu6050_dmpfw.txt"
};

static size_t imuRead(void* data, const size_t size);
static bool   imuWrite(const uint8_t address, const void* data, const size_t size);
static bool   imuRequest(const uint8_t address, size_t size, const MPU_Complete completed);
static size_t mpuDMPFirmwareRead(void* data, const size_t offset, size_t size);
static void   imuConfigure(MPU* mpu);
static void   imuInterrupt();
static void   imuPacketAvailable(const MPU* mpu, const bool success);
static void   imuPacketDataReady(const MPU* mpu, const bool success);
static void   imuInterruptTaskFunc(MPU* mpu);
static void   imuPacketAvailableTaskFunc(MPU* mpu);
static void   imuPacketDataReadyTaskFunc(MPU* mpu);

static size_t imuRead(void* data, const size_t size) { return I2C_Read(&imuDevice, data, size); }

static bool imuWrite(const uint8_t address, const void* data, const size_t size) { return I2C_WriteMem(&imuDevice, address, data, size, NULL); }

static bool imuRequest(const uint8_t address, size_t size, const MPU_Complete completed)
{
	return I2C_RequestMem(&imuDevice, address, size, (I2C_Complete)completed);
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

	GPIO_EnableIRQ(GPIO_ISR0, GPIO_INTERRUPT_TRIGGER_RISING, imuInterrupt);

	MPU_Enable(imu);
}

static void imuInterrupt() { imuInterruptTriggered = true; }

static void imuPacketAvailable(const MPU* mpu, const bool success)
{
	(void)mpu;
	if (success)
		imuPacketAvailableTriggered = true;
}

static void imuPacketDataReady(const MPU* mpu, const bool success)
{
	(void)mpu;
	if (success)
		imuPacketDataReadyTriggered = true;
}

static void imuInterruptTaskFunc(MPU* mpu)
{
	if (!imuInterruptTriggered)
		return;

	imuInterruptTriggered = false;
	MPU_RequestAvailablePackets(mpu, imuPacketAvailable);
}

static void imuPacketAvailableTaskFunc(MPU* mpu)
{
	if (!imuPacketAvailableTriggered)
		return;

	imuPacketAvailableTriggered = false;
	uint16_t numPackets         = MPU_AvailablePackets(mpu);
	// CLI_Write(cmdLine, "Available Packets: %d\n\r", numPackets);
	if (numPackets)
	{
		MPU_RequestPacket(mpu, imuPacketDataReady);
	}
}

static void imuPacketDataReadyTaskFunc(MPU* mpu)
{
	if (!imuPacketDataReadyTriggered)
		return;

	imuPacketDataReadyTriggered = false;

	Quaternion q = MPU_PacketQuaternion(mpu);
	float      r, p, y;
	Quaternion_YawPitchRoll(q, &y, &p, &r);
	// CLI_Write(cmdLine, "Y:%d P:%d R:%d\n\r", (int)RAD_TO_DEG(y), (int)RAD_TO_DEG(p), (int)RAD_TO_DEG(r));
}

void Setup_IMU()
{
	imu = &mpu6050;
	imuDevice = I2C_BindDevice(imu, i2c, IMU_DEVICE_ADDRESS, I2C_ADDRESSING_8BIT);
	MPU_Init(imu, imuRead, imuWrite, imuRequest);

	Scheduler_CreateSingleTask(taskScheduler, &imuConfigTask, IMU_CONFIG_TASK, (Scheduler_TaskFunction)imuConfigure, imu, IMU_TASK_DELAY);
	Scheduler_CreateRecurringTask(taskScheduler, &imuInterruptTask, IMU_INTERRUPT_TASK, (Scheduler_TaskFunction)imuInterruptTaskFunc, imu, 0);
	Scheduler_CreateRecurringTask(taskScheduler, &imuPacketAvailableTask, IMU_PACKET_AVAILABLE_TASK, (Scheduler_TaskFunction)imuPacketAvailableTaskFunc, imu, 0);
	Scheduler_CreateRecurringTask(taskScheduler, &imuPacketDataReadyTask, IMU_PACKET_READY_TASK, (Scheduler_TaskFunction)imuPacketDataReadyTaskFunc, imu, 0);

	Scheduler_Deactivate(&imuConfigTask);

	imuInterruptTriggered       = false;
	imuPacketAvailableTriggered = false;
	imuPacketDataReadyTriggered = false;
}
