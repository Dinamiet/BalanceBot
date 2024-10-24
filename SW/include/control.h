#ifndef _CONTROL_H_
#define _CONTROL_H_

#include <stdbool.h>
#include <stdint.h>

void Setup_Control();
void Control_SetP(int16_t value);
void Control_SetI(int16_t value);
void Control_SetD(int16_t value);
void Control_SetActive(bool active);
void Control_SetTarget(int16_t target);

#endif
