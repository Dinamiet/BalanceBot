#include "cmds.h"
#include "messages.h"

#include <getopt.h>
#include <stdio.h>

void imu_Cmd(const CLI* cli, const size_t argc, char* const argv[])
{
	enum
	{
		cmdCalGyro,
		cmdCalAccel,
		cmdSubscribe,
		cmdUnknown
	} cmd = cmdUnknown;

	char c;
	bool unsub;
	optind = 1; // Reset getopt
	while ((c = getopt(argc, argv, "gas:")) != -1)
	{
		switch (c)
		{
			case 'g':
				cmd = cmdCalGyro;
				break;

			case 'a':
				cmd = cmdCalAccel;
				break;

			case 's':
				cmd   = cmdSubscribe;
				unsub = *optarg == '0';
				break;

			default:
				cmd = cmdUnknown;
				break;
		}
	}

	switch (cmd)
	{
		case cmdCalGyro:
			printf("Calibrating gyroscope...\n\r");
			Message_Request(MESSAGE_GYRO_CALIBRATION, NULL, 0);
			break;

		case cmdCalAccel:
			printf("Calibrating accelerometer...\n\r");
			Message_Request(MESSAGE_ACCEL_CALIBRATION, NULL, 0);
			break;

		case cmdSubscribe:
			if (unsub)
				printf("Unsubscribing from IMU data...\n\r");
			else
				printf("Subscribing to IMU data...\n\r");
			Message_Request(MESSAGE_SUBSCRIBE_IMU, &unsub, sizeof(unsub));
			break;

		default:
			CLI_Write(cli, "Unknown command.\n\r");
			break;
	}
}

const char* const imu_Help[] = {
		"IMU usage:",
		"imu {gas}",
		" -g Calibrate gyroscope",
		" -a Calibrate accelerometer",
		" -s {0/1} Subscribe/Unsubscribe to IMU data",
		0,
};
