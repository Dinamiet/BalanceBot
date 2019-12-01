#include "IMU.h"

IMU::IMU(uint8_t address) { IMU_address = address; }

void IMU::writeRegister(uint8_t reg, uint8_t value)
{
	Wire.beginTransmission(IMU_address);
	Wire.write(reg);
	Wire.write(value);
	Wire.endTransmission(true);
}

void IMU::writeBytes(uint8_t reg, uint8_t* data, uint8_t len)
{
	Wire.beginTransmission(IMU_address);
	Wire.write(reg);
	for (uint8_t i = 0; i < len; i++) { Wire.write(data[i]); }
	Wire.endTransmission(true);
}

uint8_t IMU::readRegister(uint8_t reg)
{
	Wire.beginTransmission(IMU_address);
	Wire.write(reg);
	Wire.endTransmission(false);
	Wire.requestFrom(IMU_address, 1U);
	return Wire.read();
}

void IMU::readBytes(uint8_t reg, uint8_t* data, uint8_t len)
{
	Wire.beginTransmission(IMU_address);
	Wire.write(reg);
	Wire.endTransmission(false);
	Wire.requestFrom(IMU_address, len);
	for (uint8_t i = 0; i < len; i++) { data[i] = Wire.read(); }
}

void IMU::writeDMPMem()
{
	uint8_t buff[DMP_CHUNK_SIZE];
	uint8_t bank	  = 0x1F;
	uint8_t address	  = 0x00;
	uint8_t chunkSize = DMP_CHUNK_SIZE;

	//Set memory bank
	writeRegister(0x6D, bank);
	//Set memory address
	writeRegister(0x6E, address);

	for (uint16_t i = 0; i < DMP_MEM_SIZE;)
	{
		if (i + chunkSize > DMP_MEM_SIZE) //Make sure we don't go past the data size
			chunkSize = DMP_MEM_SIZE - i;

		if (chunkSize > 256 - address) // Make sure this chunk does not go past bank boundary (256 bytes)
			chunkSize = 256 - address;

		//Read values from FLASH to RAM
		for (uint8_t j = 0; j < chunkSize; j++) { buff[j] = pgm_read_byte(&(dmpMemory[i + j])); }

		//Write to IMU
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
				writeRegister(0x6D, bank);
				//Set memory address
				writeRegister(0x6E, address);
			}
		}
	}
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
	uint8_t data[6];

	readBytes(RAW_ACCEL_REG, data, 6);

	*x = (int16_t)((data[0] << 8) | data[1]) / ACCEL_SCALE;
	*y = (int16_t)((data[2] << 8) | data[3]) / ACCEL_SCALE;
	*z = (int16_t)((data[4] << 8) | data[5]) / ACCEL_SCALE;
}

void IMU::getGyro(float* x, float* y, float* z)
{
	uint8_t data[6];

	readBytes(RAW_GYRO_REG, data, 6);

	*x = (int16_t)((data[0] << 8) | data[1]) / GYRO_SCALE;
	*y = (int16_t)((data[2] << 8) | data[3]) / GYRO_SCALE;
	*z = (int16_t)((data[4] << 8) | data[5]) / GYRO_SCALE;
}

void IMU::getTemp(float* temp)
{
	uint8_t data[2];

	readBytes(RAW_TEMP_REG, data, 2);

	*temp = (int16_t)((data[0] << 8) | data[1]) / TEMP_SCALE + TEMP_OFFSET;
}
