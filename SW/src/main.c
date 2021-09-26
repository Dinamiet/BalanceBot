#include "cli.h"
#include "setup.h"

#include <avr/interrupt.h>
#include <stdbool.h>

extern CLI cli; // Defined in setup.c

int main()
{
	Setup_Serial();
	Setup_CLI();
	sei();

	while (true) { CLI_ProcessCommand(&cli, NULL); }

	return 0;
}
