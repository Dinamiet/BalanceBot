#include "drivers.h"

#define I2C_BUFFER_SIZE 64

I2C* i2c;

static uint8_t        i2cWriteBuff[I2C_BUFFER_SIZE];
static uint8_t        i2cReadBuff[I2C_BUFFER_SIZE];
static uint8_t        i2cTransBuff[3 * I2C_BUFFER_SIZE];

void Setup_I2C()
{
	i2c = I2C_GetInstance(I2C0);
	I2C_Init(i2c);
	I2C_AssignTransactionBuffer(i2c, i2cTransBuff, sizeof(i2cTransBuff));
	I2C_AssignReadBuffer(i2c, i2cReadBuff, sizeof(i2cReadBuff));
	I2C_AssignWriteBuffer(i2c, i2cWriteBuff, sizeof(i2cWriteBuff));
	I2C_SetBaud(i2c, 100000);
}
