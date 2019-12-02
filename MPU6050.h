#ifndef IMU_H
#define IMU_H

#include "Arduino.h"
#include "DMP_program.h"
#include "Wire.h"
#include "structures.h"

class MPU6050
{
	private:
	TwoWire Wire;
	uint8_t IMU_address;
	void	writeRegister(uint8_t reg, uint8_t value);
	void	writeBytes(uint8_t reg, uint8_t* data, uint8_t len);
	uint8_t readRegister(uint8_t reg);
	void	readBytes(uint8_t reg, uint8_t* data, uint8_t len);
	void	writeDMPMem();

	public:
	MPU6050(uint8_t address);
	void	 initIMU();
	void	 initDMP();
	void	 getRawAccel(float* x, float* y, float* z);
	void	 getRawGyro(float* x, float* y, float* z);
	void	 getAccel(float* x, float* y, float* z);
	void	 getGyro(float* x, float* y, float* z);
	void	 getTemp(float* temp);
	uint16_t getFIFOCount();
	uint8_t	 INT_status();
	void	 getFIFOBytes(uint8_t* data, uint8_t len);
	void	 setGyroOffset(int16_t x, int16_t y, int16_t z);
	void	 setAccelOffset(int16_t x, int16_t y, int16_t z);
	void	 getYawPitchRoll(float* yaw, float* pitch, float* roll);
};

#endif
