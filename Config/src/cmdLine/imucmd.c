#include "cmds.h"
#include "messages.h"

#include <getopt.h>

void imu_Cmd(const CLI* cli, const size_t argc, char* const argv[])
{
	enum
	{
		cmdSubscribe,
		cmdEnabled,
		cmdUnknown
	} cmd = cmdUnknown;

	char c;
	bool flag;
	optind = 1; // Reset getopt
	while ((c = getopt(argc, argv, "s:e:")) != -1)
	{
		switch (c)
		{
			case 's':
				cmd   = cmdSubscribe;
				flag  = (optarg[0] != '0');
				break;

			case 'e':
				cmd  = cmdEnabled;
				flag = (optarg[0] != '0');
				break;

			default:
				cmd = cmdUnknown;
				break;
		}
	}

	switch (cmd)
	{
		case cmdSubscribe:
			if (flag)
				CLI_Write(cli, "Subscribing to IMU data...\n\r");
			else
				CLI_Write(cli, "Unsubscribing from IMU data...\n\r");
			Message_Request(MESSAGE_SUBSCRIBE_IMU, &flag, sizeof(flag));
			break;

		case cmdEnabled:
			if (flag)
				CLI_Write(cli, "Enabling IMU...\n\r");
			else
				CLI_Write(cli, "Disabling IMU...\n\r");
			Message_Request(MESSAGE_IMU_ENABLED, &flag, sizeof(flag));
			break;

		default:
			CLI_Write(cli, "Unknown command\n\r");
			break;
	}
}

const char* const imu_Help[] = {
		"IMU usage:",
		" -s {0/1} Subscribe/Unsubscribe to IMU data",
		" -e {0/1} Enable/Disable IMU",
		0,
};
