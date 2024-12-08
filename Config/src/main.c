#include "cmdLine.h"
#include "messages.h"
#include "serialport.h"
#include "tcp.h"

#include <getopt.h>
#include <stdio.h>
#include <stdlib.h>

int main(int argc, char* argv[])
{
	enum
	{
		paramSerial,
		paramTCP,
		paramUnknown
	} param = paramUnknown;

	char c;
	optind                      = 1; // Reset getopt index
	char*            paramValue = NULL;
	MessageInterface interface;
	while ((c = getopt(argc, argv, "s:t:")) != -1)
	{
		switch (c)
		{
			case 's':
				param      = paramSerial;
				interface  = MESSAGE_INTERFACE_SERIAL;
				paramValue = optarg;
				break;

			case 't':
				param      = paramTCP;
				interface  = MESSAGE_INTERFACE_TCP;
				paramValue = optarg;
				break;

			default:
				param = paramUnknown;
				break;
		}
	}

	switch (param)
	{
		case paramSerial:
			printf("Using serial port: %s\n", paramValue);
			Serial_Setup(paramValue);
			break;

		case paramTCP:
			printf("Using TCP to host: %s\n", paramValue);
			TCP_Setup(paramValue);
			break;

		default:
			printf("Usage: %s -s <serial_port> | -t <tcp_host>\n", argv[0]);
			exit(1);
			break;
	}

	Messages_Setup(interface);
	CommandLine_Setup();

	while (true) { CLI_Process(&cmdLine); }

	return 0;
}
