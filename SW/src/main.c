#include "serial.h"
#include "setup.h"

#include <avr/interrupt.h>
#include <stdint.h>
#include <stdio.h>
#include <string.h>

char StringBuffer[64];

int main()
{
	Setup_Serial();
	sei();

	uint8_t data;
	while (true)
	{
		if (Serial_Read(Serial0, &data, 1))
			Serial_Write(Serial0, &data, 1);
	}

	return 0;
}
