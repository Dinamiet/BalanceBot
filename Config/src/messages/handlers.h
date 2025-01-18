#ifndef _HANDLERS_H_
#define _HANDLERS_H_

#include "messages.h"

void acknowledge_handler(const DataPacket* dp, const void* data, const size_t size);
void heartbeat_handler(const DataPacket* dp, const void* data, const size_t size);
void calibration_handler(const DataPacket* dp, const void* data, const size_t size);
void offset_handler(const DataPacket* dp, const void* data, const size_t size);
void imuData_handler(const DataPacket* dp, const void* data, const size_t size);
void debug_handler(const DataPacket* dp, const void* data, const size_t size);

#endif
