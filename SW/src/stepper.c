#include "stepper.h"

void Steppers_Init(Steppers* steppers, GPIO* left, GPIO* right)
{
	steppers->Left.Port	   = left;
	steppers->Left.Pos	   = 0;
	steppers->Left.Desired = 0;

	steppers->Right.Port	= right;
	steppers->Right.Pos		= 0;
	steppers->Right.Desired = 0;

	steppers->Enabled = false;

	// TODO: setup GPIO ports
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
	// TODO: Set pins to correct values
}

void Steppers_Disable(Steppers* steppers)
{
	steppers->Enabled = false;
	// TODO: Set pins to correct values
}

void Steppers_Step(Steppers* steppers)
{
	if (steppers->Left.Pos > steppers->Left.Desired)
	{
		--steppers->Left.Pos;
		// TODO: Set pins to correct values
	}
	else if (steppers->Left.Pos < steppers->Left.Desired)
	{
		++steppers->Left.Pos;
		// TODO: Set pins to correct values
	}

	if (steppers->Right.Pos > steppers->Right.Desired)
	{
		--steppers->Right.Pos;
		// TODO: Set pins to correct values
	}
	else if (steppers->Right.Pos < steppers->Right.Desired)
	{
		++steppers->Right.Pos;
		// TODO: Set pins to correct values
	}
}

void Steppers_IsMoving(Steppers* steppers) { return steppers->Left.Pos != steppers->Left.Desired || steppers->Right.Pos != steppers->Right.Desired; }
