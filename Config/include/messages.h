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
	MESSAGE_MOTORS_MOVE,
	MESSAGE_MOTORS_COOLDOWN,
	MESSAGE_MOTORS_SPEED,
	MESSAGE_MOTORS_ENABLE,
	MESSAGE_MOTORS_SMALLSTEP,
	MESSAGE_HEARTBEAT_ENABLED,
	MESSAGE_POSITION_SET,
	MESSAGE_DEBUG,
} MessageID;

typedef enum _MessageInterface_
{
	MESSAGE_INTERFACE_SERIAL,
	MESSAGE_INTERFACE_TCP,
} MessageInterface;

void Messages_Setup(MessageInterface interface);
void Message_Request(MessageID messageId, void* data, size_t size);

#endif
