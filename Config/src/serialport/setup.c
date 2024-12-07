#include "private.h"
#include "serialport.h"

#include <stdlib.h>
#include <termios.h>

int serialPort;

void Serial_Setup(char* deviceFile)
{
	// Open the serial port
	if ((serialPort = open(deviceFile, O_RDWR | O_NOCTTY)) < 0)
	{
		perror("Unable to open serial port");
		exit(-1);
	}

	// Get the current terminal settings
	struct termios options;
	cfmakeraw(&options);

	// Set the baud rate and other parameters
	cfsetispeed(&options, B115200); // Set baudrate to 115200
	cfsetospeed(&options, B115200);

	options.c_cc[VMIN]  = 1;
	options.c_cc[VTIME] = 0;

	// Apply the new settings to the serial port
	tcsetattr(serialPort, TCSAFLUSH, &options);

	// Initialize the serial port for reading
	tcdrain(serialPort);

	printf("Connected to %s\n", deviceFile);
}
