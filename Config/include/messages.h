#ifndef _MESSAGES_H_
#define _MESSAGES_H_

#include "datapacket.h"

extern DataPacket messenger;

enum
{
	MESSAGE_HEARTBEAT = 0x01,
	MESSAGE_OFFSET    = 0x10,
};

void Messages_Setup();

#endif
