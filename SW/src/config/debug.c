#include "config.h"
#include "messages.h"

void Config_SendDebug(void* data, size_t size) { DataPacket_Send(config, MESSAGE_DEBUG, data, size); }
