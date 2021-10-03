#include "stepper.h"

#define HALF_STEP

#ifdef HALF_STEP
#define STEP_SEQUENCE_LENGTH 8
const uint8_t step_sequence[STEP_SEQUENCE_LENGTH] = {0b0010, 0b1010, 0b1000, 0b1001, 0b0001, 0b0101, 0b0100, 0b0110};
#else
#define STEP_SEQUENCE_LENGTH 4
const uint8_t step_sequence[STEP_SEQUENCE_LENGTH] = {0b0010, 0b1000, 0b0001, 0b0100};
#endif

#define LEFT_MASK  0x0F
#define RIGHT_MASK 0xF0

static void setPinsLeft(GPIO* gpio, uint8_t value)
{
	uint8_t currentValue = GPIO_Read(gpio) & LEFT_MASK;
	currentValue |= value << 4;
	GPIO_Write(gpio, currentValue);
}

static void setPinsRight(GPIO* gpio, uint8_t value)
{
	uint8_t currentValue = GPIO_Read(gpio) & RIGHT_MASK;
	currentValue |= value;
	GPIO_Write(gpio, currentValue);
}

void Steppers_Init(Steppers* steppers, GPIO* left, GPIO* right)
{
	steppers->Left.Port	   = left;
	steppers->Left.Pos	   = 0;
	steppers->Left.Desired = 0;

	steppers->Right.Port	= right;
	steppers->Right.Pos		= 0;
	steppers->Right.Desired = 0;

	steppers->Enabled = false;

	GPIO_PinMode(right, 0, GPIO_OUTPUT);
	GPIO_PinMode(right, 1, GPIO_OUTPUT);
	GPIO_PinMode(right, 2, GPIO_OUTPUT);
	GPIO_PinMode(right, 3, GPIO_OUTPUT);

	GPIO_PinMode(left, 4, GPIO_OUTPUT);
	GPIO_PinMode(left, 5, GPIO_OUTPUT);
	GPIO_PinMode(left, 6, GPIO_OUTPUT);
	GPIO_PinMode(left, 7, GPIO_OUTPUT);

	Steppers_Disable(steppers);
}

void Steppers_MoveBy(Steppers* steppers, int16_t steps)
{
	steppers->Left.Desired += steps;
	steppers->Right.Desired += steps;
}

void Steppers_GoTo(Steppers* steppers, int16_t pos)
{
	steppers->Left.Desired	= pos;
	steppers->Right.Desired = pos;
}

void Steppers_SetPosition(Steppers* steppers, int16_t pos)
{
	steppers->Left.Desired	= pos;
	steppers->Right.Desired = pos;

	steppers->Left.Pos	= pos;
	steppers->Right.Pos = pos;
}

bool Steppers_IsEnabled(Steppers* steppers) { return steppers->Enabled; }

void Steppers_Enable(Steppers* steppers)
{
	steppers->Enabled = true;
	setPinsRight(steppers->Right.Port, step_sequence[(uint16_t)steppers->Right.Pos % STEP_SEQUENCE_LENGTH]);
	setPinsLeft(steppers->Left.Port, step_sequence[(uint16_t)steppers->Left.Pos % STEP_SEQUENCE_LENGTH]);
}

void Steppers_Disable(Steppers* steppers)
{
	steppers->Enabled = false;
	setPinsLeft(steppers->Left.Port, 0);
	setPinsRight(steppers->Right.Port, 0);
}

void Steppers_Step(Steppers* steppers)
{
	bool shouldUpdate = false;
	if (steppers->Left.Pos > steppers->Left.Desired)
	{
		--steppers->Left.Pos;
		shouldUpdate = true;
	}
	else if (steppers->Left.Pos < steppers->Left.Desired)
	{
		++steppers->Left.Pos;
		shouldUpdate = true;
	}

	if (steppers->Right.Pos > steppers->Right.Desired)
	{
		--steppers->Right.Pos;
		shouldUpdate = true;
	}
	else if (steppers->Right.Pos < steppers->Right.Desired)
	{
		++steppers->Right.Pos;
		shouldUpdate = true;
	}

	if (shouldUpdate)
		Steppers_Enable(steppers);
}

bool Steppers_IsMoving(Steppers* steppers) { return steppers->Left.Pos != steppers->Left.Desired || steppers->Right.Pos != steppers->Right.Desired; }
