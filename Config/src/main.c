#include "setup.h"

#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <unistd.h>

int main()
{
	printf("Hello, World!\n");

	Setup_Serial("/dev/ttyUSB1");

	while (true)
	{
		uint8_t buffer[512];
		int     bytes_read = read(serialPort, buffer, sizeof(buffer));
		for (int i = 0; i < bytes_read; i++) { printf("%02X ", buffer[i]); }
		if (bytes_read > 0)
			printf("\n");
	}

	return 0;
}
