#include "motors.h"

#include "gpio.h"
#include "tasks.h"
#include "topics.h"
#include "utilities.h"

#include <stdbool.h>
#include <stdlib.h>

#define LEFT_MASK            0x0F
#define RIGHT_MASK           0xF0
#define STEP_SEQUENCE_LENGTH 8

typedef struct _StepperMotor_
{
	GPIO*   Port;
	int16_t Position;
	int16_t Desired;
} StepperMotor;

typedef struct _Motors_
{
	StepperMotor Left;
	StepperMotor Right;
	bool         Enabled;
} Motors;

static Motors        motors;
static SchedulerTask stepMotorsTask;
static SchedulerTask motorsCooldown;
static const uint8_t step_sequence[STEP_SEQUENCE_LENGTH] = {0x02, 0x0A, 0x08, 0x09, 0x01, 0x05, 0x04, 0x06};

static void setPinsLeft(GPIO* port, int8_t value);
static void setPinsRight(GPIO* port, int8_t value);
static void moveMotorsTask(void* _);
static void cooldownMotorsTask(void* _);

static void setPinsLeft(GPIO* port, int8_t value)
{
	uint8_t currentValue = GPIO_Read(port) & LEFT_MASK;
	currentValue |= value << 4;
	GPIO_Write(port, currentValue);
}

static void setPinsRight(GPIO* port, int8_t value)
{
	uint8_t currentValue = GPIO_Read(port) & RIGHT_MASK;
	currentValue |= value;
	GPIO_Write(port, currentValue);
}

static void moveMotorsTask(void* _)
{
	(void)_;

	Motors_Step();
	if (Motors_IsMoving())
		Scheduler_Refresh(taskScheduler, &motorsCooldown);
}

static void cooldownMotorsTask(void* _)
{
	(void)_;
	Motors_Disable();
}

void Setup_Motors()
{
	Motors_Init();
	Scheduler_CreateRecurringTask(taskScheduler, &stepMotorsTask, TASK_MOTOR_STEP, &moveMotorsTask, NULL, TASK_MOTOR_STEP_PERIOD);
	Scheduler_CreateRecurringTask(taskScheduler, &motorsCooldown, TASK_MOTORS_COOLDOWN, &cooldownMotorsTask, NULL, TASK_MOTORS_COOLDOWN_DELAY);
}

void Motors_Init()
{
	motors.Left.Port     = GPIO_GetInstance(GPIO_D);
	motors.Left.Position = 0;
	motors.Left.Desired  = 0;

	motors.Right.Port     = GPIO_GetInstance(GPIO_C);
	motors.Right.Position = 0;
	motors.Right.Desired  = 0;

	motors.Enabled = false;

	GPIO_SetMode(motors.Right.Port, 0x0F, GPIO_MODE_OUTPUT);
	GPIO_SetMode(motors.Left.Port, 0xF0, GPIO_MODE_OUTPUT);

	Motors_Disable();
}

void Motors_MoveBy(int16_t steps)
{
	motors.Left.Desired += steps;
	motors.Right.Desired += steps;
}

void Motors_GoTo(int16_t pos)
{
	motors.Left.Desired  = pos;
	motors.Right.Desired = pos;
}

void Motor_SetPoisition(int16_t pos)
{
	motors.Left.Position  = pos;
	motors.Right.Position = pos;

	motors.Left.Desired  = pos;
	motors.Right.Desired = pos;
}

bool Motors_IsEnabled() { return motors.Enabled; }

void Motors_Enable()
{
	motors.Enabled = true;
	setPinsRight(motors.Right.Port, step_sequence[(uint16_t)motors.Right.Position % STEP_SEQUENCE_LENGTH]);
	setPinsLeft(motors.Left.Port, step_sequence[(uint16_t)motors.Left.Position % STEP_SEQUENCE_LENGTH]);
}

void Motors_Disable()
{
	motors.Enabled = false;
	setPinsRight(motors.Right.Port, 0);
	setPinsLeft(motors.Left.Port, 0);
}

void Motors_Step()
{
	bool    shouldUpdate = false;
	int16_t diff         = abs(motors.Left.Position - motors.Left.Desired);
	if (motors.Left.Position > motors.Left.Desired)
	{
		if (diff > 1 && (motors.Left.Position & 0x01) == 0)
			--motors.Left.Position;
		--motors.Left.Position;
		shouldUpdate = true;
	}
	else if (motors.Left.Position < motors.Left.Desired)
	{
		if (diff > 1 && (motors.Left.Position & 0x01) == 0)
			++motors.Left.Position;
		++motors.Left.Position;
		shouldUpdate = true;
	}

	diff = abs(motors.Right.Position - motors.Right.Desired);
	if (motors.Right.Position > motors.Right.Desired)
	{
		if (diff > 1 && (motors.Right.Position & 0x01) == 0)
			--motors.Right.Position;
		--motors.Right.Position;
		shouldUpdate = true;
	}
	else if (motors.Right.Position < motors.Right.Desired)
	{
		if (diff > 1 && (motors.Right.Position & 0x01) == 0)
			++motors.Right.Position;
		++motors.Right.Position;
		shouldUpdate = true;
	}

	if (shouldUpdate)
		Motors_Enable();
}

bool Motors_IsMoving() { return motors.Left.Desired != motors.Left.Position || motors.Right.Desired != motors.Right.Position; }
