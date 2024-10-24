#ifndef _CONTROL_H_
#define _CONTROL_H_

#include <stdbool.h>
#include <stdint.h>

void Setup_Control();
void Control_SetP(uint8_t value);
void Control_SetI(uint8_t value);
void Control_SetD(uint8_t value);
void Control_SetActive(bool active);

#endif
