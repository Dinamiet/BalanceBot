#include "control.h"

#include "cmds.h"

#include <stdlib.h>

void Cmd_control(const CLI* cli, const size_t argc, const char* argv[])
{
	if (argc != 3)
		return;
	uint8_t value;
	switch (argv[1][0])
	{
		case 'p':
			value = atoi(argv[2]);
			CLI_Write(cli, "Set P: %d\n\r", value);
			Control_SetP(value);
			break;

		case 'i':
			value = atoi(argv[2]);
			CLI_Write(cli, "Set I: %d\n\r", value);
			Control_SetI(value);
			break;

		case 'd':
			value = atoi(argv[2]);
			CLI_Write(cli, "Set D: %d\n\r", value);
			Control_SetD(value);
			break;

		case 'a':
			value = atoi(argv[2]);
			CLI_Write(cli, "Closed control loop: %d\n\r", value);
			Control_SetActive(value);
			break;

		default:
			break;
	}
}

const char* CmdHelp_control[] = {
		"Control adjustments",
		"p - Set Proportional gain",
		"i - Set Integral gain",
		"d - Set Derivitive gain",
		"a - Closed control loop 1 - Active 0 - Deactivate",
		0,
};
