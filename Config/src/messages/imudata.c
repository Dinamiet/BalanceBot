#include "cmdLine.h"
#include "messages.h"
#include "structures.h"

void imuData_handler(const DataPacket* dp, const void* data, const size_t size)
{
	(void)dp;
	(void)size;

	const int16_t* rawAngle = data;

	CLI_Write(&cmdLine, "Angle: %.2f\n\r", *rawAngle / 100.0);
}
