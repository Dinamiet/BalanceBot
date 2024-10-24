#include "control.h"

#include "motors.h"
#include "pid.h"
#include "topics.h"
#include "utilities.h"

#include <math.h>

#define CONTROL_PROP       90
#define CONTROL_INTEGRAL   15
#define CONTROL_DERIVATIVE 5

#define SWITCH_OFF_ANGLE (15.0f * (float)M_PI / 180.0f)

PID                  balanceControl;
ObserverSubscription balanceDataNotification;

static void controlData(float* angle);

static void controlData(float* angle)
{
	static uint8_t waitForAngle;

	if (fabsf(*angle) > SWITCH_OFF_ANGLE)
	{
		Motors_SetPosition(0);
		PID_ClearState(&balanceControl);
		waitForAngle = *angle > 0 ? 1 : 2;
	}

	waitForAngle = ((waitForAngle == 2 && *angle > 0) || (waitForAngle == 1 && *angle < 0)) ? 0 : waitForAngle;

	if (waitForAngle == 0)
	{
		float output = PID_Output(&balanceControl, *angle, 1);
		Motors_MoveBy(output);
	}
}

void Setup_Control()
{
	PID_Init(&balanceControl, CONTROL_PROP, CONTROL_INTEGRAL, CONTROL_DERIVATIVE);
	Observer_Subscribe(notifier, &balanceDataNotification, TOPIC_IMU_DATA, (Observer_Notify)controlData);
}
