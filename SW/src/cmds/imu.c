#include "imu.h"

#include "cmds.h"
#include "drivers.h"
#include "utilities.h"

static bool transferBusy(const MPU* mpu);

static bool transferBusy(const MPU* mpu)
{
	(void)mpu;
	return I2C_IsBusy(i2c);
}

void Cmd_imu(const CLI* cli, const size_t argc, const char* argv[])
{
	if (argc != 2)
		return;

	MPUOffset offset;
	switch (argv[1][0])
	{
		case 'g':
			MPU_Disable(imu);
			CLI_Write(cli, "Calibrate Gyro: ");
			MPU_CalibrateGyro(imu, transferBusy);
			MPU_RequestGyroOffset(imu, NULL);
			while (transferBusy(imu));
			offset = MPU_GyroOffset(imu);
			CLI_Write(cli, "%d %d %d\n\r", offset.X, offset.Y, offset.Z);
			MPU_Enable(imu);
			break;
		case 'a':
			MPU_Disable(imu);
			CLI_Write(cli, "Calibrate Accel: ");
			Vector g = Vector_Create(0, 0, 1);
			MPU_CalibrateAccel(imu, g, transferBusy);
			MPU_RequestAccelOffset(imu, NULL);
			while (transferBusy(imu));
			offset = MPU_AccelOffset(imu);
			CLI_Write(cli, "%d %d %d\n\r", offset.X, offset.Y, offset.Z);
			MPU_Enable(imu);
			break;
	};
}

const char* CmdHelp_imu[] = {
		"IMU Calibration and data dump",
		"g - calibrate gyro",
		"a - calibrate accel",
		0,
};
