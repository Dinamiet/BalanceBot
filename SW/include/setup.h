#ifndef _SETUP_H_
#define _SETUP_H_

#include "cli.h"

#define SERIAL_BUFFER_SIZE 64
#define SERIAL_BAUD		   115200

#define I2C_TX_BUFFER_SIZE		   32
#define I2C_TRANSATION_BUFFER_SIZE 4
#define I2C_SPEED				   100000 // Hz

#define MAX_TASKS 12
#define SUBSCRIPTION_BUFFER_SIZE 4

#define HALF_STEP 1

#define IMU_ADDRESS 0x68

#define CLI_PERIOD		 50	 // ms
#define HEARTBEAT_PERIOD 500 // ms
#define STEP_SPEED		 2	 // ms
#define STEPPER_HOLD	 250 // ms

#define GYRO_OFFSET_X  -45
#define GYRO_OFFSET_Y  -54
#define GYRO_OFFSET_Z  28
#define ACCEL_OFFSET_X -6999
#define ACCEL_OFFSET_Y 4933
#define ACCEL_OFFSET_Z 8439

#define KP 30
#define KI 5
#define KD 1

#define SWITCH_OFF_ANGLE 15.0f // degrees

#define IMU_DATA_NOTIFY "IMUData"

#define DEFINE_CMD(cmd)                                       \
	void		 Cmd_##cmd(CLI* cli, int argc, char* argv[]); \
	extern char* CmdHelp_##cmd[]

#endif
