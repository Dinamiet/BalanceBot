#include "control.h"

#include "database.h"
#include "mpu6050.h"
#include "pid.h"
#include "serial.h"
#include "steppers.h"

#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define PI (float)M_PI

extern Database db;
extern Steppers steppers;

PID balanceControl;

static void ControlData(void* data)
{
	static uint8_t waitForAngle;

	MPU6050_YPR* ypr = data;

	if ((float)fabs(ypr->Roll) > SWITCH_OFF_ANGLE * PI / 180.0f)
	{
		Steppers_SetPosition(&steppers, 0);
		PID_ClearState(&balanceControl);
		waitForAngle = ypr->Roll > 0 ? 1 : 2;
	}

	waitForAngle = ((waitForAngle == 2 && ypr->Roll > 0) || (waitForAngle == 1 && ypr->Roll < 0)) ? 0 : waitForAngle;

	if (waitForAngle == 0)
	{
		int16_t output = PID_Output(&balanceControl, ypr->Roll, 1);
		Steppers_MoveBy(&steppers, output);
	}
}

void Setup_Control()
{
	PID_Init(&balanceControl, KP, KI, KD);
	Database_Subscribe(&db, IMU_DATA_NOTIFY, NULL, ControlData);
}

void Cmd_control_p(CLI* cli, int argc, char* argv[])
{
	float p = atof(argv[1]);
	cli->Write("Control P: ");
	cli->Write(argv[1]);
	PID_ChangeP(&balanceControl, p);
}

char* CmdHelp_control_p[] = {
		0,
};

void Cmd_control_i(CLI* cli, int argc, char* argv[])
{
	float i = atof(argv[1]);
	cli->Write("Control I: ");
	cli->Write(argv[1]);
	PID_ChangeI(&balanceControl, i);
}

char* CmdHelp_control_i[] = {
		0,
};

void Cmd_control_d(CLI* cli, int argc, char* argv[])
{
	float d = atof(argv[1]);
	cli->Write("Control D: ");
	cli->Write(argv[1]);
	PID_ChangeD(&balanceControl, d);
}

char* CmdHelp_control_d[] = {
		0,
};

void AngleSubscriptionCallback(void* data)
{
	MPU6050_YPR* ypr = data;
	char		 buff[5];
	sprintf(buff, "%d\n", (int16_t)(ypr->Roll * 180.0f / PI));
	Serial_Write(Serial0, buff, strlen(buff));
}

void Cmd_sub(CLI* cli, int argc, char* argv[]) { Database_Subscribe(&db, IMU_DATA_NOTIFY, "Print", AngleSubscriptionCallback); }

char* CmdHelp_sub[] = {
		0,
};

void Cmd_unsub(CLI* cli, int argc, char* argv[])
{
	Subscription* sub = Database_FindSubscription(&db, "Print");
	if (sub)
		Database_Unsubscribe(&db, sub);
}

char* CmdHelp_unsub[] = {
		0,
};

void Cmd_angle(CLI* cli, int argc, char* argv[])
{
	float angle = atof(argv[1]);
	PID_Target(&balanceControl, angle * PI / 180.0f);

	cli->Write("Angle: ");
	cli->Write(argv[1]);
	cli->Write("\n");
}

char* CmdHelp_angle[] = {
		0,
};
