#include "drivers.h"
#include "utilities.h"

#include <avr/interrupt.h>
#include <stdbool.h>
#include <stdint.h>

int main()
{
	Setup_SystemTime();
	Setup_UART();

	sei();

	Setup_CLI();

	while (true) { CLI_Process(cli); }

	return 0;
}
