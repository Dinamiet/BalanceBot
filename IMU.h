#ifndef IMU_H
#define IMU_H

#include "Wire.h"

#define IMU_CLK_REG		0x6B
#define ACCEL_SETUP_REG 0x1C
#define GYRO_SETUP_REG	0x1B

#define ACCEL_START_REG 0x3B
#define GYRO_START_REG	0x43
#define TEMP_START_REG	0x41
#define INT_ENABLE_REG	0x38
#define SAMPLE_RATE_REG	0x19

#define IMU_CLK_VAL		0x01
#define ACCEL_SETUP_VAL 0x00
#define GYRO_SETUP_VAL	0x00
#define INT_ENABLE_VAL	0x01
#define SAMPLE_RATE_VAL	0xFF

#define ACCEL_SCALE -16384.0f
#define GYRO_SCALE	131.0f
#define TEMP_SCALE	340.0f
#define TEMP_OFFSET 36.53f

class IMU
{
	private:
	int		IMU_address;
	TwoWire Wire;	

	public:
	IMU(uint8_t address);
	void	init();
	void enableINT();
	void setSampleRate();
	void getAccel(float* x, float* y, float* z);
	void getGyro(float* x, float* y, float* z);
	void getTemp(float* temp);
};

#endif
