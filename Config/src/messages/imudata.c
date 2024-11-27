#include "messages.h"
#include "structures.h"

#include <stdio.h>

void imuData_handler(const DataPacket* dp, const void* data, const size_t size)
{
	(void)dp;
	(void)size;

	const IMUData* imu = data;

	printf("Angle: %.1f\n\r", imu->Angle / 10.0);
}
