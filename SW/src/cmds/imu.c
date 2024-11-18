#include "imu.h"

#include "cmds.h"
#include "drivers.h"
#include "topics.h"
#include "utilities.h"

#include <math.h>

#define RAD_TO_DEG(x) (x * 180.0f / (float)M_PI)

static ObserverSubscription imuDataSub;

static bool transferBusy(const MPU* mpu);
static void imuData(const float* angle);

static bool transferBusy(const MPU* mpu)
{
	(void)mpu;
	return I2C_IsBusy(&imuDevice);
}

static void imuData(const float* angle)
{
	float deg   = RAD_TO_DEG(*angle);
	int   whole = deg * 1000;
	CLI_Write(cmdLine, "Angle: %d\r\n", whole);
}

void Cmd_imu(const CLI* cli, const size_t argc, const char* argv[])
{
	if (argc != 2)
		return;

	MPUOffset offset;
	switch (argv[1][0])
	{
		case 'g':
			CLI_Write(cli, "Calibrate Gyro: ");
			MPU_CalibrateGyro(imu, transferBusy);
			MPU_RequestGyroOffset(imu, NULL);
			while (transferBusy(imu));
			offset = MPU_GyroOffset(imu);
			CLI_Write(cli, "%d %d %d\n\r", offset.X, offset.Y, offset.Z);
			break;
		case 'a':
			CLI_Write(cli, "Calibrate Accel: ");
			Vector g = Vector_Create(0, 0, 1);
			MPU_CalibrateAccel(imu, g, transferBusy);
			MPU_RequestAccelOffset(imu, NULL);
			while (transferBusy(imu));
			offset = MPU_AccelOffset(imu);
			CLI_Write(cli, "%d %d %d\n\r", offset.X, offset.Y, offset.Z);
			break;
		case 'd':
			MPU_Disable(imu);
			CLI_Write(cli, "IMU disabled\n\r");
			break;
		case 'e':
			MPU_Enable(imu);
			CLI_Write(cli, "IMU enabled\n\r");
			break;
		case 'r':
			CLI_Write(cli, "Gyro offset: ");
			MPU_RequestGyroOffset(imu, NULL);
			while (transferBusy(imu));
			offset = MPU_GyroOffset(imu);
			CLI_Write(cli, "%d %d %d\n\r", offset.X, offset.Y, offset.Z);
			CLI_Write(cli, "Accel offset: ");
			MPU_RequestAccelOffset(imu, NULL);
			while (transferBusy(imu));
			offset = MPU_AccelOffset(imu);
			CLI_Write(cli, "%d %d %d\n\r", offset.X, offset.Y, offset.Z);
			break;
		case 's':
			if (Observer_Subscribe(notifier, &imuDataSub, TOPIC_IMU_DATA, (Observer_Notify)imuData))
				CLI_Write(cli, "Subscribed to IMU data\n\r");
			break;
		case 'u':
			if (Observer_Unsubscribe(notifier, &imuDataSub))
				CLI_Write(cli, "Unsubscribed from IMU data\n\r");
			break;
	};
}

const char* CmdHelp_imu[] = {
		"IMU Calibration and data dump",
		"g - calibrate gyro",
		"a - calibrate accel",
		"r - read offsets",
		"e - enable IMU",
		"d - disable IMU",
		"s - Sub to output",
		"u - Unsub to output",
		0,
};
