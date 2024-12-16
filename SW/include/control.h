#ifndef _CONTROL_H_
#define _CONTROL_H_

#include <stdbool.h>
#include <stdint.h>

typedef enum _ControlTerm_
{
	CONTROL_TERM_P,
	CONTROL_TERM_I,
	CONTROL_TERM_D,
	CONTROL_TARGET,
} ControlTerm;

typedef struct _ControlValue_
{
	uint16_t Term;
	int16_t  Value;
} ControlValue;

void Setup_Control();
void Control_SetP(int16_t value);
void Control_SetI(int16_t value);
void Control_SetD(int16_t value);
void Control_SetActive(bool active);
void Control_SetTarget(int16_t target);

void Position_SetP(int16_t value);
void Position_SetI(int16_t value);
void Position_SetD(int16_t value);
void Position_SetTarget(int16_t target);

void Control_Debug(int32_t currPos, float currAngle, float targetAngle);

#endif
