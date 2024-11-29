#ifndef _MOTORS_H_
#define _MOTORS_H_

#include <stdbool.h>

void Setup_Motors();
void Motors_MoveBy(int steps);
void Motors_Reset();
void Motors_Enabled(bool enabled);
void Motors_CooldownEnabled(bool enabled);

#endif
