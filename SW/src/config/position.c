#include "control.h"
#include "messages.h"

void positionSet_Handler(const DataPacket* dp, const void* data, const size_t size)
{
	(void)size;
	const ControlValue* controlValue = data;

	switch (controlValue->Term)
	{
		case CONTROL_TERM_P:
			Position_SetP(controlValue->Value);
			break;
		case CONTROL_TERM_I:
			Position_SetI(controlValue->Value);
			break;
		case CONTROL_TERM_D:
			Position_SetD(controlValue->Value);
			break;
		case CONTROL_TARGET:
			Position_SetTarget(controlValue->Value);
			break;
	}

	DataPacket_Send(dp, MESSAGE_ACKNOWLEDGE, NULL, 0);
}
