#include "motors.h"

#include "drivers.h"
#include "stepper.h"
#include "tasks.h"
#include "topics.h"
#include "utilities.h"

#define LEFT_MASK            0x0F
#define RIGHT_MASK           0xF0
#define STEP_SEQUENCE_LENGTH 8

#define MOTOR_SPEED 2

static Stepper left;
static Stepper right;

static SchedulerTask updateMotorsTask;
static SchedulerTask motorsCooldownTask;
static const uint8_t stepSequence[STEP_SEQUENCE_LENGTH] = {0x02, 0x0A, 0x08, 0x09, 0x01, 0x05, 0x04, 0x06};

static uint8_t stepIndexRight = 0;
static uint8_t stepIndexLeft  = 0;

static void stepLeft(bool forward);
static void stepRight(bool forward);
static void enableLeft(bool enable);
static void enableRight(bool enable);
static void updateMotorsFunc(void* _);
static void cooldownMotorsFunc(void* _);

static void stepLeft(bool forward)
{
	stepIndexLeft += forward ? 1 : -1;
	enableLeft(true);
}

static void stepRight(bool forward)
{
	stepIndexRight += forward ? 1 : -1;
	enableRight(true);
}

static void enableLeft(bool enable)
{
	GPIO*   port         = GPIO_GetInstance(GPIO_D);
	uint8_t currentValue = GPIO_Read(port) & LEFT_MASK;
	uint8_t value        = 0;
	if (enable)
		value = stepSequence[stepIndexLeft % STEP_SEQUENCE_LENGTH];
	currentValue |= value << 4;
	GPIO_Write(port, currentValue);
}

static void enableRight(bool enable)
{
	GPIO*   port         = GPIO_GetInstance(GPIO_C);
	uint8_t currentValue = GPIO_Read(port) & RIGHT_MASK;
	uint8_t value        = 0;
	if (enable)
		value = stepSequence[stepIndexRight % STEP_SEQUENCE_LENGTH];
	currentValue |= value;
	GPIO_Write(port, currentValue);
}

static void updateMotorsFunc(void* _)
{
	(void)_;

	Stepper_Run(&left, System_GetTime());
	Stepper_Run(&right, System_GetTime());
	if (Stepper_IsRunning(&left) || Stepper_IsRunning(&right))
		Scheduler_Refresh(taskScheduler, &motorsCooldownTask);
}

static void cooldownMotorsFunc(void* _)
{
	(void)_;
	Stepper_Disable(&left);
	Stepper_Disable(&right);
}

void Setup_Motors()
{
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
