#include "datapacket.h"
#include "handlers.h"
#include "messages.h"
#include "pthread.h"
#include "serialport.h"

DataPacket messenger;

static pthread_t         receive_thread;
static DataPacketMessage messages[] = {
		{MESSAGE_HEARTBEAT, heartbeat_handler},
		{				0,			  NULL}
};

static size_t read_dp(void* data, const size_t size);
static size_t write_dp(const void* data, const size_t size);
static void*  message_receive(void* arg);

static size_t read_dp(void* data, const size_t size) { return read(serialPort, data, size); }

static size_t write_dp(const void* data, const size_t size) { return write(serialPort, data, size); }

static void* message_receive(void* arg)
{
	(void)arg;
	while (true) { DataPacket_Receive(&messenger); }
	return NULL;
}

void Messages_Setup()
{
	DataPacket_Init(&messenger, messages, read_dp, write_dp);
	pthread_create(&receive_thread, NULL, message_receive, NULL);
}
