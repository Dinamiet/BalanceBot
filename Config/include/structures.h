#ifndef _STRUCTURES_H_
#define _STRUCTURES_H_

#include <stdint.h>

typedef struct _IMUData_
{
	int16_t Angle;
} IMUData;

typedef struct _ControlValue_
{
	enum
	{
		CONTROL_TERM_P,
		CONTROL_TERM_I,
		CONTROL_TERM_D,
		CONTROL_TARGET_ANGLE,
	} ControlTerm;
	int16_t Value;
} ControlValue;

typedef struct _OffsetValues_
{
	int16_t X;
	int16_t Y;
	int16_t Z;
} OffsetValues;

#endif
