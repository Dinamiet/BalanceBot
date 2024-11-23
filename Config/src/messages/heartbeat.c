#include "messages.h"

#include <stdio.h>

void heartbeat_handler(const DataPacket* dp, const void* data, const size_t size)
{
	(void)dp;
	(void)data;
	(void)size;

	printf("Heartbeat\n");
}
