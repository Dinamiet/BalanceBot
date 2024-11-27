#ifndef _MESSAGES_H_
#define _MESSAGES_H_

#include "datapacket.h"

extern DataPacket messenger;

typedef enum _MessageID_
{
	MESSAGE_HEARTBEAT = 0x01,
	MESSAGE_GYRO_CALIBRATION,
	MESSAGE_ACCEL_CALIBRATION,
	MESSAGE_SUBSCRIBE_IMU,
	MESSAGE_IMU_DATA,
} MessageID;

void Messages_Setup();
void Message_Request(MessageID messageId, void* data, size_t size);

#endif
