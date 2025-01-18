#ifndef _STRUCTURES_H_
#define _STRUCTURES_H_

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

typedef struct _OffsetValues_
{
	int16_t X;
	int16_t Y;
	int16_t Z;
} OffsetValues;

#endif
