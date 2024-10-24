#ifndef _STEPPER_H_
#define _STEPPER_H_

#include <stdbool.h>
#include <stdint.h>

void Setup_Motors();

void Motors_Init();
void Motors_MoveBy(int16_t steps);
void Motors_GoTo(int16_t pos);
void Motors_SetPosition(int16_t pos);
bool Motors_IsEnabled();
void Motors_Enable();
void Motors_Disable();
void Motors_Step();
bool Motors_IsMoving();

#endif
