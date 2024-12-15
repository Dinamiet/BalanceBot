#include "control.h"

#include "motors.h"
#include "pid.h"
#include "tasks.h"
#include "topics.h"
#include "utilities.h"

#include <math.h>

#define CONTROL_PROP_BALANCE       125
#define CONTROL_INTEGRAL_BALANCE   25
#define CONTROL_DERIVATIVE_BALANCE 10

#define CONTROL_PROP_POS       DEG_TO_RAD(-0.1f)
#define CONTROL_INTEGRAL_POS   0
#define CONTROL_DERIVATIVE_POS 0

#define DEG_TO_RAD(x) (x * (float)M_PI / 180.0f)

#define SWITCH_OFF_ANGLE     (15.0f)
#define CONTROL_TARGET_ANGLE (0.0f)
#define CONTROL_TARGET_POS   (0.0f)

PID                  balanceControl;
PID                  positionControl;
ObserverSubscription balanceDataNotification;
SchedulerTask        delayControlNotify;

static void controlData(float* angle);
static void delayedControlClose(void* _);

static void controlData(float* angle)
{
	static uint8_t waitForAngle = 1;

	if (fabsf(*angle) > DEG_TO_RAD(SWITCH_OFF_ANGLE))
	{
		Motors_Reset();
		PID_ClearState(&balanceControl);
		PID_ClearState(&positionControl);
		waitForAngle = *angle > 0 ? 1 : 2;
	}

	waitForAngle = ((waitForAngle == 2 && *angle > 0) || (waitForAngle == 1 && *angle < 0)) ? 0 : waitForAngle;

	if (waitForAngle == 0)
	{
		float output;
		output = PID_Output(&positionControl, Motors_CurrentPosition(), 1);
		PID_Target(&balanceControl, output);

		output = PID_Output(&balanceControl, *angle, 1);
		Motors_MoveBy(output);
	}
}

static void delayedControlClose(void* _)
{
	(void)_;
	Observer_Subscribe(notifier, &balanceDataNotification, TOPIC_IMU_DATA, (Observer_NotifyHandler)controlData);
}

void Setup_Control()
{
	PID_Init(&balanceControl, CONTROL_PROP_BALANCE, CONTROL_INTEGRAL_BALANCE, CONTROL_DERIVATIVE_BALANCE);
	PID_Target(&balanceControl, DEG_TO_RAD(CONTROL_TARGET_ANGLE));
	PID_WindupPrevention(&balanceControl, true, 0.10f);

	PID_Init(&positionControl, CONTROL_PROP_POS, CONTROL_INTEGRAL_POS, CONTROL_DERIVATIVE_POS);
	PID_Target(&positionControl, 0.0f);
	PID_WindupPrevention(&positionControl, true, DEG_TO_RAD(1.0f));

	Scheduler_CreateSingleTask(taskScheduler, &delayControlNotify, TASK_DELAY_CONTROL, delayedControlClose, NULL, TASK_DELAY_CONTROL_TIME);
}

void Control_SetP(int16_t value) { PID_SetProportional(&balanceControl, (float)value); }

void Control_SetI(int16_t value) { PID_SetIntegral(&balanceControl, (float)value); }

void Control_SetD(int16_t value) { PID_SetDerivative(&balanceControl, (float)value); }

void Control_SetActive(bool active)
{
	if (active & !Observer_HasSubscription(notifier, &balanceDataNotification))
	{
		delayedControlClose(NULL);
	}
	else if (!active)
	{
		Observer_Unsubscribe(notifier, &balanceDataNotification);
	}
}

void Control_SetTarget(int16_t target)
{
	float angle = DEG_TO_RAD(((float)target) / 100.0f);
	PID_Target(&balanceControl, angle);
}

void Position_SetP(int16_t value) { PID_SetProportional(&positionControl, (float)value); }

void Position_SetI(int16_t value) { PID_SetIntegral(&positionControl, (float)value); }

void Position_SetD(int16_t value) { PID_SetDerivative(&positionControl, (float)value); }

void Position_SetTarget(int16_t target) { PID_Target(&positionControl, target); }
