#include "cmds.h"
#include "messages.h"

#include <getopt.h>

void accel_Cmd(const CLI* cli, const size_t argc, char* const argv[])
{
	enum
	{
		cmdCalibrate,
		cmdOffset,
		cmdUnknown
	} cmd = cmdUnknown;

	char c;
	optind = 1; // Reset getopt index
	while ((c = getopt(argc, argv, "co")) != -1)
	{
		switch (c)
		{
			case 'c':
				cmd = cmdCalibrate;
				break;

			case 'o':
				cmd = cmdOffset;
				break;

			default:
				cmd = cmdUnknown;
				break;
		}
	}

	switch (cmd)
	{
		case cmdCalibrate:
			CLI_Write(cli, "Accel calibration...\n\r");
			Message_Request(MESSAGE_ACCEL_CALIBRATION, NULL, 0);
			break;

		case cmdOffset:
			CLI_Write(cli, "Accel offset...\n\r");
			Message_Request(MESSAGE_ACCEL_OFFSET, NULL, 0);
			break;

		default:
			CLI_Write(cli, "Unknown command\n\r");
			break;
	}
}

const char* const accel_Help[] = {
		"Accel usage:",
		" -c Calibrate",
		" -o Get offset",
		0,
};
