#include "cmds.h"
#include "messages.h"

#include <getopt.h>
#include <stdio.h>

void motor_Cmd(const CLI* cli, const size_t argc, char* const argv[])
{
	enum
	{
		cmdEnable,
		cmdCooldown,
		cmdMove,
		cmdUnknown
	} cmd = cmdUnknown;

	char    c;
	bool    flag;
	int16_t steps;
	optind = 1;
	while ((c = getopt(argc, argv, "e:c:m:")) != -1)
	{
		switch (c)
		{
			case 'e':
				cmd  = cmdEnable;
				flag = (optarg[0] != '0');
				break;

			case 'c':
				cmd  = cmdCooldown;
				flag = (optarg[0] != '0');
				break;

			case 'm':
				cmd = cmdMove;
				sscanf(optarg, "%hd", &steps);
				break;

			default:
				cmd = cmdUnknown;
				break;
		}
	}

	switch (cmd)
	{
		case cmdCooldown:
			CLI_Write(cli, "Motor cooldown: %s\n\r", flag ? "Enabled" : "Disabled");
			Message_Request(MESSAGE_MOTORS_COOLDOWN, &flag, sizeof(flag));
			break;

		case cmdEnable:
			CLI_Write(cli, "Motor enable: %s\n\r", flag ? "Enabled" : "Disabled");
			Message_Request(MESSAGE_MOTORS_ENABLE, &flag, sizeof(flag));
			break;

		case cmdMove:
			CLI_Write(cli, "Motor move: %d\n\r", steps);
			Message_Request(MESSAGE_MOTORS_MOVE, &steps, sizeof(steps));
			break;

		default:
			CLI_Write(cli, "Unknown command\n\r");
			break;
	}
}

const char* const motor_Help[] = {
		"Motor usage:",
		" -e {1|0} Enable/Disable motor",
		" -c {1|0} Enable/Disable motor cooldown",
		" -m {steps} Move motor by specified steps",
		0,
};
