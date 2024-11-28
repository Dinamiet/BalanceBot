#include "cmds.h"
#include "messages.h"

#include <getopt.h>

void gyro_Cmd(const CLI* cli, const size_t argc, char* const argv[])
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
			CLI_Write(cli, "Gyro calibration - ");
			Message_Request(MESSAGE_GYRO_CALIBRATION, NULL, 0);
			break;

		case cmdOffset:
			CLI_Write(cli, "Gyro offset - ");
			Message_Request(MESSAGE_GYRO_OFFSET, NULL, 0);
			break;

		case cmdUnknown:
			CLI_Write(cli, "Unknown command\n");
			break;
	}
}

const char* const gyro_Help[] = {
		"Gyro usage:",
		" -c Calibrate",
		" -o Get offset",
		0,
};
