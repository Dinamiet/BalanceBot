#include "cmds.h"
#include "messages.h"

#include <getopt.h>

void debug_Cmd(const CLI* cli, const size_t argc, char* const argv[])
{
	enum
	{
		cmdEnable,
		cmdUnknown
	} cmd = cmdUnknown;

	char c;
	optind = 1; // Reset getopt index
	bool flag;
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
			CLI_Write(cli, "Debug %s\n\r", flag ? "Enabled" : "Disabled");
			Message_Request(MESSAGE_DEBUG, &flag, sizeof(flag));
			break;

		default:
			break;
	}
}

const char* const debug_Help[] = {
		"Debug usage:",
		" -e {1|0} Enable/Disable Debug mode",
		0,
};
