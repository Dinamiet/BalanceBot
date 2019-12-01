#include "IMU.h"

IMU::IMU(uint8_t address) { IMU_address = address; }

void IMU::writeRegister(uint8_t reg, uint8_t value)
{
	Wire.beginTransmission(IMU_address);
	Wire.write(reg);
	Wire.write(value);
	Wire.endTransmission(true);
}

void IMU::writeDMPMem() {}

void IMU::init()
{
	initIMU();
	// initDMP();
}

void IMU::initIMU()
{
	Wire.begin();
	Wire.setClock(400000);

	writeRegister(0x6B, 0x01); //Setup clock source
	writeRegister(0x23, 0x00); //No FIFO
	writeRegister(0x38, 0x00); //No INT
	writeRegister(0x37, 0x80); //INT Active LOW
	writeRegister(0x1C, 0x00); //Accelerometer SCALE
	writeRegister(0x1B, 0x18); //Gyro SCALE
	writeRegister(0x19, 0x04); //Sample RATE
	writeRegister(0x1A, 0x01); //Digital Filter
}

void IMU::initDMP()
{
	writeDMPMem();

	writeRegister(0x70, 0x04); //Program start Address
	writeRegister(0x71, 0x00); //Program start Address

	writeRegister(0x38, 0x02); //Enable INT
	writeRegister(0x6A, 0xC4); //Enable DMP & FIFO and reset FIFO.
}

void IMU::getAccel(float* x, float* y, float* z)
{
	Wire.beginTransmission(IMU_address);
	Wire.write(ACCEL_START_REG);
	Wire.endTransmission(false);
	Wire.requestFrom(IMU_address, 6);

	*x = ((int16_t)(Wire.read() << 8) | Wire.read()) / ACCEL_SCALE;
	*y = ((int16_t)(Wire.read() << 8) | Wire.read()) / ACCEL_SCALE;
	*z = ((int16_t)(Wire.read() << 8) | Wire.read()) / ACCEL_SCALE;
}

void IMU::getGyro(float* x, float* y, float* z)
{
	Wire.beginTransmission(IMU_address);
	Wire.write(GYRO_START_REG);
	Wire.endTransmission(false);
	Wire.requestFrom(IMU_address, 6);

	*y = ((int16_t)(Wire.read() << 8) | Wire.read()) / GYRO_SCALE;
	*x = ((int16_t)(Wire.read() << 8) | Wire.read()) / GYRO_SCALE;
	*z = ((int16_t)(Wire.read() << 8) | Wire.read()) / GYRO_SCALE;
}

void IMU::getTemp(float* temp)
{
	Wire.beginTransmission(IMU_address);
	Wire.write(RAW_TEMP_REG);
	Wire.endTransmission(false);
	Wire.requestFrom(IMU_address, 2);

	*temp = ((int16_t)(Wire.read() << 8) | Wire.read()) / TEMP_SCALE + TEMP_OFFSET;
}

void IMU::enableINT()
{
	Wire.beginTransmission(IMU_address);
	Wire.write(INT_ENABLE_REG);
	Wire.write(INT_ENABLE_VAL);
	Wire.endTransmission(true);
}

void IMU::setSampleRate()
{
	Wire.beginTransmission(IMU_address);
	Wire.write(SAMPLE_RATE_REG);
	Wire.write(SAMPLE_RATE_VAL);
	Wire.endTransmission(true);
}
