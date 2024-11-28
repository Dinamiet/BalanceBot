#ifndef _MESSAGES_H_
#define _MESSAGES_H_

#include "datapacket.h"

extern DataPacket messenger;

typedef enum _MessageID_
{
	MESSAGE_ACKNOWLEDGE,
	MESSAGE_HEARTBEAT,
	MESSAGE_GYRO_CALIBRATION,
	MESSAGE_GYRO_OFFSET,
	MESSAGE_ACCEL_CALIBRATION,
	MESSAGE_ACCEL_OFFSET,
	MESSAGE_SUBSCRIBE_IMU,
	MESSAGE_IMU_DATA,
	MESSAGE_IMU_ENABLED,
	MESSAGE_CONTROL_SET,
	MESSAGE_CONTROL_ENABLE,
} MessageID;

void Messages_Setup();
void Message_Request(MessageID messageId, void* data, size_t size);

#endif
