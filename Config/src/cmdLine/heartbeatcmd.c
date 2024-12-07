#include "cmds.h"
#include "messages.h"

#include <getopt.h>

void heartbeat_Cmd(const CLI* cli, const size_t argc, char* const argv[])
{
	enum
	{
		cmdEnable,
		cmdUnknown
	} cmd = cmdUnknown;

	char c;
	bool flag;
	optind = 1; // Reset getopt index
	while ((c = getopt(argc, argv, "e:")) != -1)
	{
		switch (c)
		{
			case 'e':
				cmd  = cmdEnable;
				flag = (optarg[0] != '0');
				break;

			default:
				cmd = cmdUnknown;
				break;
		}
	}

	switch (cmd)
	{
		case cmdEnable:
			if (flag)
				CLI_Write(cli, "Enable heartbeat...\n\r");
			else
				CLI_Write(cli, "Disable heartbeat...\n\r");
			Message_Request(MESSAGE_HEARTBEAT_ENABLED, &flag, sizeof(flag));
			break;

		default:
			CLI_Write(cli, "Unknown command\n\r");
			break;
	}
}

const char* const heartbeat_Help[] = {
		"Heartbeat usage:",
		" -e {0|1} Enable/Disable heartbeat",
		0,
};
