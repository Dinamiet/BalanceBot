#include "imu.h"

#include "messages.h"
#include "topics.h"
#include "utilities.h"

#include <math.h>

#define RAD_TO_DEG(x) (x * 180.0f / (float)M_PI)
#define DEG_TO_RAD(x) (x * (float)M_PI / 180.0f)

ObserverSubscription imuDataSub;

static void imuData(const float* angle)
{
	float   deg   = RAD_TO_DEG(*angle);
	int16_t whole = deg * 100;
	DataPacket_Send(config, MESSAGE_IMU_DATA, &whole, sizeof(whole));
}

void subscribeIMURequest_Handler(const DataPacket* dp, const void* data, const size_t size)
{
	(void)size;

	bool flag = *(bool*)data;

	if (flag)
	{
		if (!Observer_HasSubscription(notifier, &imuDataSub))
			Observer_Subscribe(notifier, &imuDataSub, TOPIC_IMU_DATA, (Observer_NotifyHandler)imuData);
	}

	DataPacket_Send(dp, MESSAGE_ACKNOWLEDGE, NULL, 0);
}

void imuEnabled_Handler(const DataPacket* dp, const void* data, const size_t size)

{
	(void)size;

	bool flag = *(bool*)data;

	MPU_Enable(imu, flag);

	DataPacket_Send(dp, MESSAGE_ACKNOWLEDGE, NULL, 0);
}
