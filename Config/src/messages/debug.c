#include "cmdLine.h"
#include "messages.h"

typedef struct _ControlDebug_
{
	int32_t CurrentPosition;
	int16_t CurrentAngle;
	int16_t TargetAngle;
} ControlDebug;

void debug_handler(const DataPacket* dp, const void* data, const size_t size)
{
	(void)dp;
	(void)size;

	ControlDebug* debugData = (ControlDebug*)data;

	CLI_Write(&cmdLine,
	          "\r\tCurrent Position: %d\tCurrent Angle: %.2f\tTarget Angle: %.2f\n\r",
	          debugData->CurrentPosition,
	          (float)debugData->CurrentAngle / 100.0,
	          (float)debugData->TargetAngle / 100.0);
}
