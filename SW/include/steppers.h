#ifndef _STEPPER_H_
#define _STEPPER_H_

#include "gpio.h"

#include <stdbool.h>

typedef struct
{
	GPIO*	Port;
	int16_t Pos;
	int16_t Desired;
} StepperMotor;

typedef struct
{
	StepperMotor Left;
	StepperMotor Right;
	bool		 Enabled;
} Steppers;

void Steppers_Init(Steppers* steppers, GPIO* left, GPIO* right);
void Steppers_MoveBy(Steppers* steppers, int16_t steps);
void Steppers_GoTo(Steppers* steppers, int16_t pos);
void Steppers_SetPosition(Steppers* steppers, int16_t pos);
bool Steppers_IsEnabled(Steppers* steppers);
void Steppers_Enable(Steppers* steppers);
void Steppers_Disable(Steppers* steppers);
void Steppers_Step(Steppers* steppers);
bool Steppers_IsMoving(Steppers* steppers);

#endif
