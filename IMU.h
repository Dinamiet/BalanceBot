#ifndef IMU_H
#define IMU_H

#include "Arduino.h"
#include "DMP_MEM.h"
#include "Wire.h"

#define RAW_ACCEL_REG 0x3B
#define RAW_GYRO_REG  0x43
#define RAW_TEMP_REG  0x41

#define ACCEL_SCALE -16384.0f
#define GYRO_SCALE	16.4f
#define TEMP_SCALE	340.0f
#define TEMP_OFFSET 36.53f

class IMU
{
	private:
	uint8_t IMU_address;
	TwoWire Wire;
	void	writeRegister(uint8_t reg, uint8_t value);
	void	writeBytes(uint8_t reg, uint8_t* data, uint8_t len);
	uint8_t readRegister(uint8_t reg);
	void	readBytes(uint8_t reg, uint8_t* data, uint8_t len);
	void	writeDMPMem();

	public:
	IMU(uint8_t address);
	void	 initIMU();
	void	 initDMP();
	void	 getAccel(float* x, float* y, float* z);
	void	 getGyro(float* x, float* y, float* z);
	void	 getTemp(float* temp);
	uint16_t getFIFOCount();
	uint8_t	 INT_status();
	void	 getFIFOBytes(uint8_t* data, uint8_t len);
	void	 setGyroOffset(int16_t x, int16_t y, int16_t z);
	void	 setAccelOffset(int16_t x, int16_t y, int16_t z);
};

#endif
