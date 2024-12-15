#ifndef _MOTORS_H_
#define _MOTORS_H_

#include <stdbool.h>
#include <stdint.h>

void    Setup_Motors();
void    Motors_MoveBy(int16_t steps);
int32_t Motors_CurrentPosition();
void    Motors_Reset();
void    Motors_Enabled(bool enabled);
void    Motors_CooldownEnabled(bool enabled);
void    Motors_StepSize(bool smallStep);
void    Motors_Speed(uint32_t speed);

#endif
