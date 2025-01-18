#include "cmds.h"
#include "messages.h"
#include "structures.h"

#include <getopt.h>
#include <math.h>
#include <stdio.h>

#define DEG_TO_RAD(x) (x * (float)M_PI / 180.0f)

void pos_Cmd(const CLI* cli, const size_t argc, char* const argv[])
{
	enum
	{
		cmdSetP,
		cmdSetI,
		cmdSetD,
		cmdEnableControl,
		cmdSetTarget,
		cmdUnknown
	} cmd = cmdUnknown;

	char c;
	optind = 1; // Reset getopt
	float   value;
	bool    flag;
	while ((c = getopt(argc, argv, "p:i:d:a:t:")) != -1)
	{
		switch (c)
		{
			case 'p':
				cmd = cmdSetP;
				sscanf(optarg, "%f", &value);
				break;

			case 'i':
				cmd = cmdSetI;
				sscanf(optarg, "%f", &value);
				break;

			case 'd':
				cmd = cmdSetD;
				sscanf(optarg, "%f", &value);
				break;

			case 'a':
				cmd  = cmdEnableControl;
				flag = (optarg[0] != '0');
				break;

			case 't':
				cmd = cmdSetTarget;
				sscanf(optarg, "%f", &value);
				break;

			default:
				cmd = cmdUnknown;
				break;
		}

		ControlValue controlValue;

		switch (cmd)
		{
			case cmdSetP:
				controlValue.Term  = CONTROL_TERM_P;
				controlValue.Value = DEG_TO_RAD(value) * 1000.0f;
				CLI_Write(cli, "Setting Proportional gain to %f\n\r", value);
				Message_Request(MESSAGE_POSITION_SET, &controlValue, sizeof(controlValue));
				break;

			case cmdSetI:
				controlValue.Term  = CONTROL_TERM_I;
				controlValue.Value = DEG_TO_RAD(value) * 1000.0f;
				CLI_Write(cli, "Setting Integral gain to %f\n\r", value);
				Message_Request(MESSAGE_POSITION_SET, &controlValue, sizeof(controlValue));
				break;

			case cmdSetD:
				controlValue.Term  = CONTROL_TERM_D;
				controlValue.Value = DEG_TO_RAD(value) * 1000.0f;
				CLI_Write(cli, "Setting Derivative gain to %f\n\r", value);
				Message_Request(MESSAGE_POSITION_SET, &controlValue, sizeof(controlValue));
				break;

			case cmdEnableControl:
				CLI_Write(cli, "Enabling control: %s\n\r", flag ? "true" : "false");
				Message_Request(MESSAGE_CONTROL_ENABLE, &flag, sizeof(flag));
				break;

			case cmdSetTarget:
				controlValue.Term  = CONTROL_TARGET;
				controlValue.Value = (int16_t)value;
				Message_Request(MESSAGE_POSITION_SET, &controlValue, sizeof(controlValue));
				break;

			default:
				CLI_Write(cli, "Unknown command\n\r");
				break;
		}
	}
}

const char* const pos_Help[] = {
		"Position usage:",
		" -p {value} Set Proportinal gain",
		" -i {value} Set Integral gain",
		" -d {value} Set Derivative gain",
		" -a {0|1} Enable/Disable closed loop control",
		" -t {deg} Set target position",
		0,
};
