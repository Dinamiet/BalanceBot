#ifndef _MESSAGES_H_
#define _MESSAGES_H_

#include "datapacket.h"

extern DataPacket* config;

enum
{
	CONFIG_MESSAGE_HEARTBEAT = 0x01,
	CONFIG_MESSAGE_OFFSET    = 0x10,
};

void offsetRequest_Handler(const DataPacket* dp, const void* data, const size_t size);

#endif
