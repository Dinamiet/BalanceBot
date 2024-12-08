#include "datapacket.h"
#include "handlers.h"
#include "messages.h"
#include "pthread.h"
#include "serialport.h"
#include "tcp.h"

DataPacket messenger;

static pthread_t         receive_thread;
static DataPacketMessage messages[] = {
		{      MESSAGE_ACKNOWLEDGE, acknowledge_handler},
		{        MESSAGE_HEARTBEAT,   heartbeat_handler},
		{ MESSAGE_GYRO_CALIBRATION, calibration_handler},
		{      MESSAGE_GYRO_OFFSET,      offset_handler},
		{MESSAGE_ACCEL_CALIBRATION, calibration_handler},
		{     MESSAGE_ACCEL_OFFSET,      offset_handler},
		{		 MESSAGE_IMU_DATA,     imuData_handler},
		{						0,				NULL}
};

static void*  message_receive(void* arg);

static void* message_receive(void* arg)
{
	(void)arg;
	while (true) { DataPacket_Receive(&messenger); }
	return NULL;
}

void Messages_Setup(MessageInterface interface)
{
	DataPacket_ReadInterface  dp_read;
	DataPacket_WriteInterface dp_write;
	switch (interface)
	{
		case MESSAGE_INTERFACE_SERIAL:
			dp_read  = Serial_Read;
			dp_write = Serial_Write;
			break;

		case MESSAGE_INTERFACE_TCP:
			dp_read  = TCP_Read;
			dp_write = TCP_Write;
			break;

			// No default - all options has to be handled by switch
	}

	DataPacket_Init(&messenger, messages, dp_read, dp_write);
	pthread_create(&receive_thread, NULL, message_receive, NULL);
}
