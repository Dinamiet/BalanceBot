#include "IMU.h"

IMU::IMU(uint8_t address)
{
	IMU_address= address;
}

void IMU::init()
{
	Wire.begin();

	Wire.beginTransmission(IMU_address);
	Wire.write(ACCEL_SETUP_REG);
	Wire.write(ACCEL_SETUP_VAL);
	Wire.endTransmission(true);

	Wire.beginTransmission(IMU_address);
	Wire.write(GYRO_SETUP_REG);
	Wire.write(GYRO_SETUP_VAL);
	Wire.endTransmission(true);

	Wire.beginTransmission(IMU_address);
	Wire.write(IMU_CLK_REG);
	Wire.write(IMU_CLK_VAL);
	Wire.endTransmission(true);
}

void IMU::getAccel(float* x, float* y, float* z)
{
	Wire.beginTransmission(IMU_address);
	Wire.write(ACCEL_START_REG);
	Wire.endTransmission(false);
	Wire.requestFrom(IMU_address, 6);

	*x= ((int16_t)(Wire.read() << 8) | Wire.read())/ACCEL_SCALE;
	*y= ((int16_t)(Wire.read() << 8) | Wire.read())/ACCEL_SCALE;
	*z= ((int16_t)(Wire.read() << 8) | Wire.read())/ACCEL_SCALE;
}

void IMU::getGyro(float* x, float* y, float* z)
{
	Wire.beginTransmission(IMU_address);
	Wire.write(GYRO_START_REG);
	Wire.endTransmission(false);
	Wire.requestFrom(IMU_address, 6);

	*y= ((int16_t)(Wire.read() << 8) | Wire.read())/GYRO_SCALE;
	*x= ((int16_t)(Wire.read() << 8) | Wire.read())/GYRO_SCALE;
	*z= ((int16_t)(Wire.read() << 8) | Wire.read())/GYRO_SCALE;
}

void IMU::getTemp(float* temp)
{
	Wire.beginTransmission(IMU_address);
	Wire.write(TEMP_START_REG);
	Wire.endTransmission(false);
	Wire.requestFrom(IMU_address, 2);

	*temp= ((int16_t)(Wire.read() << 8) | Wire.read())/TEMP_SCALE + TEMP_OFFSET;
}
