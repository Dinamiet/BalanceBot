#include "config.h"
#include "imu.h"
#include "messages.h"

static void gyroOffset_Ready(const MPU* mpu, const bool success);
static void accelOffset_Ready(const MPU* mpu, const bool success);

static void gyroOffset_Ready(const MPU* mpu, const bool success)
{
	if (success)
	{
		MPUOffset offset = MPU_GyroOffset(mpu);
		DataPacket_Send(config, MESSAGE_GYRO_OFFSET, &offset, sizeof(offset));
	}
}

static void accelOffset_Ready(const MPU* mpu, const bool success)
{
	if (success)
	{
		MPUOffset offset = MPU_AccelOffset(mpu);
		DataPacket_Send(config, MESSAGE_ACCEL_OFFSET, &offset, sizeof(offset));
	}
}

void gyroOffsetRequest_Handler(const DataPacket* dp, const void* data, const size_t size)
{
	(void)dp;
	(void)data;
	(void)size;

	MPU_RequestGyroOffset(imu, gyroOffset_Ready);
}

void accelOffsetRequest_Handler(const DataPacket* dp, const void* data, const size_t size)
{
	(void)dp;
	(void)data;
	(void)size;

	MPU_RequestAccelOffset(imu, accelOffset_Ready);
}
