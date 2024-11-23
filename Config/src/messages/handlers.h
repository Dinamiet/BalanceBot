#ifndef _HANDLERS_H_
#define _HANDLERS_H_

#include "messages.h"

void heartbeat_handler(const DataPacket* dp, const void* data, const size_t size);

#endif
