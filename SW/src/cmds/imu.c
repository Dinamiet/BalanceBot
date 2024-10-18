#include "imu.h"

#include "cmds.h"
#include "drivers.h"
#include "utilities.h"

static bool transferBusy(const MPU* mpu);

static bool transferBusy(const MPU* mpu)
{
	(void)mpu;
	return I2C_IsBusy(&imuDevice);
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
	};
}

const char* CmdHelp_imu[] = {
		"IMU Calibration and data dump",
		"g - calibrate gyro",
		"a - calibrate accel",
		"r - read offsets",
		"e - enable IMU",
		"d - disable IMU",
		0,
};
