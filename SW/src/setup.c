#include "setup.h"

#include "serial.h"

// Serial setup
#define SERIAL_BUFFER_SIZE 128
uint8_t TXBuff_serial[64];
uint8_t RXBuff_serial[64];

#define SERIAL_BAUD 115200
void Setup_Serial()
{
	Serial_Configure(Serial0, SERIAL_8N1, SERIAL_BAUD);
	Serial_InitTXBuffer(Serial0, TXBuff_serial, sizeof(TXBuff_serial));
	Serial_InitRXBuffer(Serial0, RXBuff_serial, sizeof(RXBuff_serial));
	Serial_Enable(Serial0);
}
