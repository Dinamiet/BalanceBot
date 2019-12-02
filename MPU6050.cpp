#include "MPU6050.h"

MPU6050::MPU6050(uint8_t address) { IMU_address = address; }

void MPU6050::writeRegister(uint8_t reg, uint8_t value)
{
	Wire.beginTransmission(IMU_address);
	Wire.write(reg);
	Wire.write(value);
	Wire.endTransmission(true);
}

void MPU6050::writeBytes(uint8_t reg, uint8_t* data, uint8_t len)
{
	Wire.beginTransmission(IMU_address);
	Wire.write(reg);
	for (uint8_t i = 0; i < len; i++) { Wire.write(data[i]); }
	Wire.endTransmission(true);
}

uint8_t MPU6050::readRegister(uint8_t reg)
{
	Wire.beginTransmission(IMU_address);
	Wire.write(reg);
	Wire.endTransmission(false);
	Wire.requestFrom(IMU_address, 1U);
	return Wire.read();
}

void MPU6050::readBytes(uint8_t reg, uint8_t* data, uint8_t len)
{
	Wire.beginTransmission(IMU_address);
	Wire.write(reg);
	Wire.endTransmission(false);
	Wire.requestFrom(IMU_address, len);
	for (uint8_t i = 0; i < len; i++) { data[i] = Wire.read(); }
}

void MPU6050::writeDMPMem()
{
	uint8_t buff[DMP_CHUNK_SIZE];
	uint8_t bank	  = 0;
	uint8_t address	  = 0x00;
	uint8_t chunkSize = DMP_CHUNK_SIZE;

	//Set memory bank
	writeRegister(0x6D, bank & 0x1F);
	//Set memory address
	writeRegister(0x6E, address);

	for (uint16_t i = 0; i < DMP_MEM_SIZE;)
	{
		if (i + chunkSize > DMP_MEM_SIZE) //Make sure we don't go past the data size
			chunkSize = DMP_MEM_SIZE - i;

		if (chunkSize > 256 - address) // Make sure this chunk does not go past bank boundary (256 bytes)
			chunkSize = 256 - address;

		//Read values from FLASH to RAM
		for (uint8_t j = 0; j < chunkSize; j++) { buff[j] = pgm_read_byte(&(DMP_Program[i + j])); }

		//Write to MPU
		writeBytes(0x6F, buff, chunkSize);

		// Step for next data
		i += chunkSize;
		address += chunkSize;

		if (i < DMP_MEM_SIZE)
		{
			if (address == 0)
			{
				bank++;
				//Set memory bank
				writeRegister(0x6D, bank & 0x1F);
				//Set memory address
				writeRegister(0x6E, address);
			}
		}
	}
}

void MPU6050::initIMU()
{
	Wire.begin();
	Wire.setClock(400000);

	writeRegister(0x6B, 0x80); //Reset MPU
	delay(100);

	writeRegister(0x6B, 0x01); //Setup clock source
	writeRegister(0x23, 0x00); //No FIFO
	writeRegister(0x38, 0x00); //No INT
	writeRegister(0x37, 0x80); //INT Active LOW
	writeRegister(0x1C, 0x00); //Accelerometer SCALE
	writeRegister(0x1B, 0x18); //Gyro SCALE
	writeRegister(0x19, 0x04); //Sample RATE
	writeRegister(0x1A, 0x01); //Digital Filter
}

void MPU6050::initDMP()
{
	writeDMPMem();

	//Program Start address
	uint8_t startAddress[] = {0x04, 0x00};
	writeBytes(0x70, startAddress, 2);

	writeRegister(0x38, 0x02); //Enable DMP INT
	writeRegister(0x6A, 0xC4); //Enable DMP & FIFO and reset FIFO.
}

void MPU6050::getAccel(float* x, float* y, float* z)
{
	uint8_t data[6];

	readBytes(0x3B, data, 6);

	*x = (int16_t)((data[0] << 8) | data[1]) / ACCEL_SCALE;
	*y = (int16_t)((data[2] << 8) | data[3]) / ACCEL_SCALE;
	*z = (int16_t)((data[4] << 8) | data[5]) / ACCEL_SCALE;
}

void MPU6050::getGyro(float* x, float* y, float* z)
{
	uint8_t data[6];

	readBytes(0x43, data, 6);

	*x = (int16_t)((data[0] << 8) | data[1]) / GYRO_SCALE;
	*y = (int16_t)((data[2] << 8) | data[3]) / GYRO_SCALE;
	*z = (int16_t)((data[4] << 8) | data[5]) / GYRO_SCALE;
}

void MPU6050::getTemp(float* temp)
{
	uint8_t data[2];

	readBytes(0x41, data, 2);

	*temp = (int16_t)((data[0] << 8) | data[1]) / TEMP_SCALE + TEMP_OFFSET;
}

uint16_t MPU6050::getFIFOCount()
{
	uint8_t data[2];
	readBytes(0x72, data, 2);
	return (data[0] << 8) | data[1];
}

uint8_t MPU6050::INT_status() { return readRegister(0x3A); }

void MPU6050::getFIFOBytes(uint8_t* data, uint8_t len) { readBytes(0x74, data, len); }

void MPU6050::setGyroOffset(int16_t x, int16_t y, int16_t z)
{
	uint8_t offsets[6] = {(x >> 8) & 0xFF, x & 0xFF, (y >> 8) & 0xFF, y & 0xFF, (z >> 8) & 0xFF, z & 0xFF};
	writeBytes(0x13, offsets, 6);
}

void MPU6050::setAccelOffset(int16_t x, int16_t y, int16_t z)
{
	uint8_t offsets[6] = {(x >> 8) & 0xFF, x & 0xFF, (y >> 8) & 0xFF, y & 0xFF, (z >> 8) & 0xFF, z & 0xFF};
	writeBytes(0x06, offsets, 6);
}
