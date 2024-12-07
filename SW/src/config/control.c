#include "control.h"

#include "messages.h"

typedef enum _ControlTerm_
{
	CONTROL_TERM_P,
	CONTROL_TERM_I,
	CONTROL_TERM_D,
	CONTROL_TARGET_ANGLE,
} ControlTerm;

typedef struct _ControlValue_
{
	uint16_t Term;
	int16_t  Value;
} ControlValue;

void controlSet_Handler(const DataPacket* dp, const void* data, const size_t size)
{
	(void)size;
	const ControlValue* controlValue = data;

	switch (controlValue->Term)
	{
		case CONTROL_TERM_P:
			Control_SetP(controlValue->Value);
			break;
		case CONTROL_TERM_I:
			Control_SetI(controlValue->Value);
			break;
		case CONTROL_TERM_D:
			Control_SetD(controlValue->Value);
			break;
		case CONTROL_TARGET_ANGLE:
			Control_SetTarget(controlValue->Value);
	}

	DataPacket_Send(dp, MESSAGE_ACKNOWLEDGE, NULL, 0);
}

void controlEnable_Handler(const DataPacket* dp, const void* data, const size_t size)
{
	(void)size;

	bool flag = *(bool*)data;

	Control_SetActive(flag);

	DataPacket_Send(dp, MESSAGE_ACKNOWLEDGE, NULL, 0);
}
