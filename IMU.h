#ifndef IMU_H
#define IMU_H

#include "Wire.h"

#define IMU_CLK_REG		0x6B
#define ACCEL_SETUP_REG 0x1C
#define GYRO_SETUP_REG	0x1B

#define ACCEL_START_REG 0x3B
#define GYRO_START_REG	0x43
#define TEMP_START_REG	0x41

#define IMU_CLK_VAL		0x00
#define ACCEL_SETUP_VAL 0x00
#define GYRO_SETUP_VAL	0x00

#define ACCEL_SCALE -16384.0f
#define GYRO_SCALE	131.0f
#define TEMP_SCALE	340.0f
#define TEMP_OFFSET 36.53f

class IMU
{
	private:
	int		IMU_address;
	TwoWire Wire;
	void	init();

	public:
	IMU(uint8_t address);
	void getAccel(float* x, float* y, float* z);
	void getGyro(float* x, float* y, float* z);
	void getTemp(float* temp);
};

#endif
