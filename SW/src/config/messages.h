#ifndef _MESSAGES_H_
#define _MESSAGES_H_

#include "datapacket.h"

extern DataPacket* config;

enum
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
	MESSAGE_MOTORS_ENABLE,
	MESSAGE_MOTORS_SMALLSTEP,
	MESSAGE_HEARTBEAT_ENABLED,
};

void gyroCalibrationRequest_Handler(const DataPacket* dp, const void* data, const size_t size);
void gyroOffsetRequest_Handler(const DataPacket* dp, const void* data, const size_t size);
void accelCalibrationRequest_Handler(const DataPacket* dp, const void* data, const size_t size);
void accelOffsetRequest_Handler(const DataPacket* dp, const void* data, const size_t size);
void subscribeIMURequest_Handler(const DataPacket* dp, const void* data, const size_t size);
void imuEnabled_Handler(const DataPacket* dp, const void* data, const size_t size);
void controlSet_Handler(const DataPacket* dp, const void* data, const size_t size);
void controlEnable_Handler(const DataPacket* dp, const void* data, const size_t size);
void motorsMove_Handler(const DataPacket* dp, const void* data, const size_t size);
void motorsCooldown_Handler(const DataPacket* dp, const void* data, const size_t size);
void motorsEnable_Handler(const DataPacket* dp, const void* data, const size_t size);
void motorsSmallStep_Handler(const DataPacket* dp, const void* data, const size_t size);
void heartbeatEnable_Hander(const DataPacket* dp, const void* data, const size_t size);

#endif
