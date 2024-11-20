#include "config.h"
#include "imu.h"
#include "messages.h"

static void offsetReq_Ready(const MPU* mpu, const bool success);

static void offsetReq_Ready(const MPU* mpu, const bool success)
{
	if (success)
	{
		MPUOffset offset = MPU_GyroOffset(mpu);
		DataPacket_Send(config, CONFIG_MESSAGE_OFFSET, &offset, sizeof(offset));
	}
}

void offsetRequest_Handler(const DataPacket* dp, const void* data, const size_t size)
{
	(void)dp;
	(void)data;
	(void)size;

	MPU_RequestGyroOffset(imu, offsetReq_Ready);
}
