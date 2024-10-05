#include "drivers.h"

#include <avr/interrupt.h>
#include <stdbool.h>
#include <stdint.h>

int main()
{
	Setup_UART();

	sei();

	char msg[] = "Hello World\n\r";
	while (true)
	{
		UART_Write(uart, msg, sizeof(msg));
		while (UART_IsBusy(uart));
	}

	return 0;
}
