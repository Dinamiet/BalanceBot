#include "cmds.h"
#include "messages.h"
#include "structures.h"

#include <getopt.h>
#include <stdio.h>

void control_Cmd(const CLI* cli, const size_t argc, char* const argv[])
{
	enum
	{
		cmdSetP,
		cmdSetI,
		cmdSetD,
		cmdEnableControl,
		cmdSetTargetAngle,
		cmdUnknown
	} cmd = cmdUnknown;

	char c;
	optind = 1; // Reset getopt
	int16_t value;
	float   angle;
	bool  flag;
	while ((c = getopt(argc, argv, "p:i:d:a:t:")) != -1)
	{
		switch (c)
		{
			case 'p':
				cmd = cmdSetP;
				sscanf(optarg, "%hd", &value);
				break;

			case 'i':
				cmd = cmdSetI;
				sscanf(optarg, "%hd", &value);
				break;

			case 'd':
				cmd = cmdSetD;
				sscanf(optarg, "%hd", &value);
				break;

			case 'a':
				cmd  = cmdEnableControl;
				flag = (optarg[0] != '0');
				break;

			case 't':
				cmd = cmdSetTargetAngle;
				sscanf(optarg, "%f", &angle);
				break;

			default:
				cmd = cmdUnknown;
				break;
		}
	}

	ControlValue controlValue;

	switch (cmd)
	{
		case cmdSetP:
			controlValue.Term  = CONTROL_TERM_P;
			controlValue.Value = value;
			CLI_Write(cli, "Setting Proportional gain to %d\n\r", controlValue.Value);
			Message_Request(MESSAGE_CONTROL_SET, &controlValue, sizeof(controlValue));
			break;

		case cmdSetI:
			controlValue.Term  = CONTROL_TERM_I;
			controlValue.Value = value;
			CLI_Write(cli, "Setting Integral gain to %d\n\r", controlValue.Value);
			Message_Request(MESSAGE_CONTROL_SET, &controlValue, sizeof(controlValue));
			break;

		case cmdSetD:
			controlValue.Term  = CONTROL_TERM_D;
			controlValue.Value = value;
			CLI_Write(cli, "Setting Derivative gain to %d\n\r", controlValue.Value);
			Message_Request(MESSAGE_CONTROL_SET, &controlValue, sizeof(controlValue));
			break;

		case cmdEnableControl:
			CLI_Write(cli, "Enabling control: %s\n\r", flag ? "true" : "false");
			Message_Request(MESSAGE_CONTROL_ENABLE, &flag, sizeof(flag));
			break;

		case cmdSetTargetAngle:
			CLI_Write(cli, "Setting target angle to %f\n\r", angle);
			controlValue.Term  = CONTROL_TARGET_ANGLE;
			controlValue.Value = angle * 100.0f;
			Message_Request(MESSAGE_CONTROL_SET, &controlValue, sizeof(controlValue));
			break;

		default:
			CLI_Write(cli, "Unknown command\n\r");
			break;
	}
}

const char* const control_Help[] = {
		"Control Usage:",
		" -p {value} Set Proportinal gain",
		" -i {value} Set Integral gain",
		" -d {value} Set Derivative gain",
		" -a {0|1} Enable/Disable closed loop control",
		" -t {deg} Set target angle in degrees",
		0,
};
