#include "cmdLine.h"
#include "messages.h"
#include "structures.h"

void imuData_handler(const DataPacket* dp, const void* data, const size_t size)
{
	(void)dp;
	(void)size;

	const IMUData* imu = data;

	CLI_Write(&cmdLine, "Angle: %.1f\n\r", imu->Angle / 10.0);
}
