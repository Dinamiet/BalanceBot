#include "messages.h"

void Message_Request(MessageID messageId, void* data, size_t size) { DataPacket_Send(&messenger, messageId, data, size); }
