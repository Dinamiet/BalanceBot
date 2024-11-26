#include "cmdLine.h"
#include "messages.h"
#include "serialport.h"

int main()
{
	Serial_Setup("/dev/ttyUSB1");
	Messages_Setup();
	CommandLine_Setup();

	while (true) { CLI_Process(&cmdLine); }

	return 0;
}
