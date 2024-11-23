#include "messages.h"
#include "serialport.h"

int main()
{
	printf("Hello, World!\n");

	Serial_Setup("/dev/ttyUSB1");
	Messages_Setup();

	while (true) { sleep(1000); }

	return 0;
}
