#include "control.h"

#include "motors.h"
#include "pid.h"
#include "topics.h"
#include "utilities.h"

#include <math.h>

#define CONTROL_PROP       60
#define CONTROL_INTEGRAL   10
#define CONTROL_DERIVATIVE 2

#define DEG_TO_RAD(x) (x * (float)M_PI / 180.0f)

#define SWITCH_OFF_ANGLE (15.0f)

PID                  balanceControl;
ObserverSubscription balanceDataNotification;

static void controlData(float* angle);

static void controlData(float* angle)
{
	static uint8_t waitForAngle;

	if (fabsf(*angle) > DEG_TO_RAD(SWITCH_OFF_ANGLE))
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
	PID_Target(&balanceControl, 0);
	Observer_Subscribe(notifier, &balanceDataNotification, TOPIC_IMU_DATA, (Observer_Notify)controlData);
}

void Control_SetP(int16_t value) { PID_SetProportional(&balanceControl, value); }

void Control_SetI(int16_t value) { PID_SetIntegral(&balanceControl, value); }

void Control_SetD(int16_t value) { PID_SetDerivative(&balanceControl, value); }

void Control_SetActive(bool active)
{
	if (active & !Observer_HasSubscription(notifier, &balanceDataNotification))
	{
		Observer_Subscribe(notifier, &balanceDataNotification, TOPIC_IMU_DATA, (Observer_Notify)controlData);
	}
	else if (!active)
	{
		Observer_Unsubscribe(notifier, &balanceDataNotification);
	}
}

void Control_SetTarget(int16_t target)
{
	float angle = DEG_TO_RAD(((float)target) / 10.0f);
	PID_Target(&balanceControl, angle);
}
