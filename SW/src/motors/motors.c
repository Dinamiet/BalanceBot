#include "motors.h"

#include "drivers.h"
#include "stepper.h"
#include "tasks.h"
#include "topics.h"
#include "utilities.h"

#define ENABLE_PIN    0
#define DIRECTION_PIN 1
#define STEP_PIN      2

#define ENABLE_MASK    (1 << ENABLE_PIN)
#define DIRECTION_MASK (1 << DIRECTION_PIN)
#define STEP_MASK      (1 << STEP_PIN)
#define LEFT_OFFSET    4

#define MOTOR_SPEED 10

static Stepper left;
static Stepper right;

static SchedulerTask updateMotorsTask;
static SchedulerTask motorsCooldownTask;

static void stepLeft(bool forward);
static void stepRight(bool forward);
static void enableLeft(bool enable);
static void enableRight(bool enable);
static void updateMotorsFunc(void* _);
static void cooldownMotorsFunc(void* _);

static void stepLeft(bool forward)
{
	GPIO* gpio = GPIO_GetInstance(GPIO_D);
	GPIO_WritePin(gpio, DIRECTION_PIN + LEFT_OFFSET, forward);
	GPIO_TogglePin(gpio, STEP_PIN + LEFT_OFFSET);
}

static void stepRight(bool forward)
{
	GPIO* gpio = GPIO_GetInstance(GPIO_C);
	GPIO_WritePin(gpio, DIRECTION_PIN, forward);
	GPIO_TogglePin(gpio, STEP_PIN);
}

static void enableLeft(bool enable)
{
	GPIO* gpio = GPIO_GetInstance(GPIO_D);
	GPIO_WritePin(gpio, ENABLE_PIN + LEFT_OFFSET, !enable);
}

static void enableRight(bool enable)
{
	GPIO* gpio = GPIO_GetInstance(GPIO_C);
	GPIO_WritePin(gpio, ENABLE_PIN, !enable);
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

static void cooldownMotorsFunc(void* _)
{
	(void)_;
	Stepper_Disable(&left);
	Stepper_Disable(&right);
}

void Setup_Motors()
{
	const uint8_t modeMask = ENABLE_MASK | DIRECTION_MASK | STEP_MASK;
	GPIO*         gpio     = NULL;

	// Right
	gpio = GPIO_GetInstance(GPIO_C);
	GPIO_SetMode(gpio, modeMask, GPIO_MODE_OUTPUT);
	GPIO_Write(gpio, 0);

	// Left
	gpio = GPIO_GetInstance(GPIO_D);
	GPIO_SetMode(gpio, modeMask << LEFT_OFFSET, GPIO_MODE_OUTPUT);
	GPIO_Write(gpio, 0);

	Stepper_Init(&left, stepLeft, enableLeft);
	Stepper_Init(&right, stepRight, enableRight);

	Stepper_SetSpeed(&left, MOTOR_SPEED);
	Stepper_SetSpeed(&right, MOTOR_SPEED);

	Scheduler_CreateRecurringTask(taskScheduler, &updateMotorsTask, TASK_MOTOR_STEP, &updateMotorsFunc, NULL, TASK_MOTOR_STEP_PERIOD);
	Scheduler_CreateRecurringTask(taskScheduler, &motorsCooldownTask, TASK_MOTORS_COOLDOWN, &cooldownMotorsFunc, NULL, TASK_MOTORS_COOLDOWN_DELAY);
}

void Motors_MoveBy(int steps)
{
	Stepper_Move(&left, steps);
	Stepper_Move(&right, steps);
}

void Motors_Reset()
{
	Stepper_SetPosition(&left, Stepper_TargetPosition(&left));
	Stepper_SetPosition(&right, Stepper_TargetPosition(&right));
}
