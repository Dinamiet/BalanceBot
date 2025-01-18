#include "motors.h"

#include "drivers.h"
#include "stepper.h"
#include "tasks.h"
#include "topics.h"
#include "utilities.h"

#define RIGHT_GPIO          GPIO_C
#define RIGHT_ENABLE_PIN    3
#define RIGHT_SPEED_PIN     2
#define RIGHT_STEP_PIN      1
#define RIGHT_DIRECTION_PIN 0

#define LEFT_GPIO          GPIO_D
#define LEFT_ENABLE_PIN    4
#define LEFT_SPEED_PIN     5
#define LEFT_STEP_PIN      6
#define LEFT_DIRECTION_PIN 7

#define MOTOR_SPEED 1

static Stepper left;
static Stepper right;

static SchedulerTask updateMotorsTask;
static SchedulerTask stepPinClearTask;
static SchedulerTask motorsCooldownTask;

static void stepLeft(bool forward);
static void stepRight(bool forward);
static void enableLeft(bool enable);
static void enableRight(bool enable);
static void updateMotorsFunc(void* _);
static void pinClearFunc(void* _);
static void cooldownMotorsFunc(void* _);

static void stepLeft(bool forward)
{
	GPIO* gpio = GPIO_GetInstance(LEFT_GPIO);
	GPIO_WritePin(gpio, LEFT_DIRECTION_PIN, forward);
	GPIO_WritePin(gpio, LEFT_STEP_PIN, 1);
}

static void stepRight(bool forward)
{
	GPIO* gpio = GPIO_GetInstance(RIGHT_GPIO);
	GPIO_WritePin(gpio, RIGHT_DIRECTION_PIN, forward);
	GPIO_WritePin(gpio, RIGHT_STEP_PIN, 1);
}

static void enableLeft(bool enable)
{
	GPIO* gpio = GPIO_GetInstance(LEFT_GPIO);
	GPIO_WritePin(gpio, LEFT_ENABLE_PIN, !enable);
}

static void enableRight(bool enable)
{
	GPIO* gpio = GPIO_GetInstance(RIGHT_GPIO);
	GPIO_WritePin(gpio, RIGHT_ENABLE_PIN, !enable);
}

static void updateMotorsFunc(void* _)
{
	(void)_;

	Stepper_Run(&left, System_GetTime());
	Stepper_Run(&right, System_GetTime());
	if (Stepper_IsRunning(&left) || Stepper_IsRunning(&right))
	{
		Stepper_Enable(&left);
		Stepper_Enable(&right);
		Scheduler_Refresh(taskScheduler, &motorsCooldownTask);
	}
}

static void pinClearFunc(void* _)
{
	(void)_;

	GPIO* gpio;

	// Left
	gpio = GPIO_GetInstance(LEFT_GPIO);
	GPIO_WritePin(gpio, LEFT_STEP_PIN, 0);

	// Right
	gpio = GPIO_GetInstance(RIGHT_GPIO);
	GPIO_WritePin(gpio, RIGHT_STEP_PIN, 0);
}

static void cooldownMotorsFunc(void* _)
{
	(void)_;
	Stepper_Disable(&left);
	Stepper_Disable(&right);
}

void Setup_Motors()
{
	uint8_t modeMask = 0;
	GPIO*   gpio     = NULL;

	// Right
	modeMask = (1 << RIGHT_DIRECTION_PIN | 1 << RIGHT_ENABLE_PIN | 1 << RIGHT_STEP_PIN | 1 << RIGHT_SPEED_PIN);
	gpio     = GPIO_GetInstance(GPIO_C);
	GPIO_SetMode(gpio, modeMask, GPIO_MODE_OUTPUT);
	GPIO_Write(gpio, 1 << RIGHT_SPEED_PIN | 1 << RIGHT_ENABLE_PIN); // Set small step and disabled

	// Left
	modeMask = (1 << LEFT_DIRECTION_PIN | 1 << LEFT_ENABLE_PIN | 1 << LEFT_STEP_PIN | 1 << LEFT_SPEED_PIN);
	gpio     = GPIO_GetInstance(GPIO_D);
	GPIO_SetMode(gpio, modeMask, GPIO_MODE_OUTPUT);
	GPIO_Write(gpio, 1 << LEFT_SPEED_PIN | 1 << LEFT_ENABLE_PIN); // Set small step and disabled

	Stepper_Init(&left, stepLeft, enableLeft);
	Stepper_Init(&right, stepRight, enableRight);

	Stepper_SetSpeed(&left, MOTOR_SPEED);
	Stepper_SetSpeed(&right, MOTOR_SPEED);

	Scheduler_CreateRecurringTask(taskScheduler, &updateMotorsTask, TASK_MOTOR_STEP, &updateMotorsFunc, NULL, TASK_MOTOR_STEP_PERIOD);
	Scheduler_CreateRecurringTask(taskScheduler, &stepPinClearTask, TASK_STEP_PIN_CLEAR, &pinClearFunc, NULL, TASK_MOTOR_STEP_PERIOD);
	Scheduler_CreateRecurringTask(taskScheduler, &motorsCooldownTask, TASK_MOTORS_COOLDOWN, &cooldownMotorsFunc, NULL, TASK_MOTORS_COOLDOWN_DELAY);
}

void Motors_MoveBy(int steps)
{
	Stepper_Move(&left, steps);
	Stepper_Move(&right, steps);
}

int32_t Motors_CurrentPosition() { return (Stepper_CurrentPosition(&left) + Stepper_CurrentPosition(&right)) / 2; }

void Motors_Reset()
{
	Stepper_SetPosition(&left, 0);
	Stepper_SetPosition(&right, 0);

	Stepper_MoveTo(&left, 0);
	Stepper_MoveTo(&right, 0);
}

void Motors_Enabled(bool enabled)
{
	if (enabled)
	{
		Stepper_Enable(&left);
		Stepper_Enable(&right);
	}
	else
	{
		Stepper_Disable(&left);
		Stepper_Disable(&right);
	}
}

void Motors_CooldownEnabled(bool enabled)
{
	if (enabled)
		Scheduler_Activate(&motorsCooldownTask);
	else
		Scheduler_Deactivate(&motorsCooldownTask);
}

void Motors_StepSize(bool smallStep)
{
	GPIO* gpio;
	gpio = GPIO_GetInstance(RIGHT_GPIO);
	GPIO_WritePin(gpio, RIGHT_SPEED_PIN, smallStep);

	gpio = GPIO_GetInstance(LEFT_GPIO);
	GPIO_WritePin(gpio, LEFT_SPEED_PIN, smallStep);
}

void Motors_Speed(uint32_t speed)
{
	Stepper_SetSpeed(&left, speed);
	Stepper_SetSpeed(&right, speed);
}
