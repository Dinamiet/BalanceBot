#include "config.h"
#include "messages.h"

void heartbeatEnable_Hander(const DataPacket* dp, const void* data, const size_t size)
{
	(void)size;
	bool flag = *(bool*)data;

	Heartbeat_Enabled(flag);

	DataPacket_Send(dp, MESSAGE_ACKNOWLEDGE, NULL, 0);
}
