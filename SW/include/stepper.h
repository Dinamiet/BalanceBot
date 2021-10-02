#ifndef _STEPPER_H_
#define _STEPPER_H_

#include "GPIO.h"

// Left Stepper -> Arduino Pins - 6, 7, 4, 5 -> PD6, PD7 PD4, PD5
// Right Stepper -> Arduino Pins - A2, A3, A0, A1 -> PC2, PC3, PC0, PC1

// PD -> 4, 5, 6, 7
// PC -> 0, 1, 2, 3

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
void Steppers_IsMoving(Steppers* steppers);

#endif
