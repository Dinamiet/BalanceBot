#include "cmdLine.h"
#include "messages.h"

void heartbeat_handler(const DataPacket* dp, const void* data, const size_t size)
{
	(void)dp;
	(void)data;
	(void)size;

	CLI_Write(&cmdLine, "Heartbeat\n");
}
