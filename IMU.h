#ifndef IMU_H
#define IMU_H

#include "Wire.h"

#define ACCEL_START_REG 0x3B
#define GYRO_START_REG	0x43
#define TEMP_START_REG	0x41

#define ACCEL_SCALE -16384.0f
#define GYRO_SCALE	16.4f
#define TEMP_SCALE	340.0f
#define TEMP_OFFSET 36.53f

class IMU
{
	private:
	int		IMU_address;
	TwoWire Wire;
	void	writeRegister(uint8_t reg, uint8_t value);
	void	writeDMPMem();

	public:
	IMU(uint8_t address);
	void init();
	void enableINT();
	void setSampleRate();
	void getAccel(float* x, float* y, float* z);
	void getGyro(float* x, float* y, float* z);
	void getTemp(float* temp);
};

#endif
