#include "motors.h"

#include "config.h"
#include "messages.h"

void motorsMove_Handler(const DataPacket* dp, const void* data, const size_t size)
{
	(void)size;

	int16_t steps = *(int16_t*)data;

	Motors_MoveBy(steps);

	DataPacket_Send(dp, MESSAGE_ACKNOWLEDGE, NULL, 0);
}

void motorsCooldown_Handler(const DataPacket* dp, const void* data, const size_t size)
{
	(void)size;

	bool flag = *(bool*)data;

	Motors_CooldownEnabled(flag);

	DataPacket_Send(dp, MESSAGE_ACKNOWLEDGE, NULL, 0);
}

void motorsEnable_Handler(const DataPacket* dp, const void* data, const size_t size)
{
	(void)size;
	bool flag = *(bool*)data;

	Motors_Enabled(flag);

	DataPacket_Send(dp, MESSAGE_ACKNOWLEDGE, NULL, 0);
}
