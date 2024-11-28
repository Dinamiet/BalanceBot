#include "config.h"
#include "imu.h"
#include "messages.h"

static bool transferBusy(const MPU* mpu);

static bool transferBusy(const MPU* mpu)
{
	(void)mpu;
	return I2C_IsBusy(&imuDevice);
}

void gyroCalibrationRequest_Handler(const DataPacket* dp, const void* data, const size_t size)
{
	(void)data;
	(void)size;

	MPU_CalibrateGyro(imu, transferBusy);

	DataPacket_Send(dp, MESSAGE_GYRO_CALIBRATION, NULL, 0);
}

void accelCalibrationRequest_Handler(const DataPacket* dp, const void* data, const size_t size)
{
	(void)data;
	(void)size;

	Vector g = Vector_Create(0, 0, 1);
	MPU_CalibrateAccel(imu, g, transferBusy);

	DataPacket_Send(dp, MESSAGE_ACCEL_CALIBRATION, NULL, 0);
}
