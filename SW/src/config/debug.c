#include "config.h"
#include "control.h"
#include "messages.h"

void Config_SendDebug(void* data, size_t size) { DataPacket_Send(config, MESSAGE_DEBUG, data, size); }

void debug_Handler(const DataPacket* dp, const void* data, const size_t size)
{
	(void)size;

	bool flag = *(bool*)data;

	Control_DebugEnable(flag);

	DataPacket_Send(dp, MESSAGE_ACKNOWLEDGE, NULL, 0);
}
