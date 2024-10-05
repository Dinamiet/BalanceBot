#include "uart.h"

#include <avr/interrupt.h>
#include <stdbool.h>
#include <stdint.h>

#define UART_BUFFER_SIZE 32

static uint8_t uartTxBuff[UART_BUFFER_SIZE];
static uint8_t uartRxBuff[UART_BUFFER_SIZE];

int main()
{
	UART* uart = UART_GetInstance(UART0);

	UART_Init(uart);
	UART_AssignReceiveBuffer(uart, uartRxBuff, sizeof(uartRxBuff));
	UART_AssignTransmitBuffer(uart, uartTxBuff, sizeof(uartTxBuff));
	UART_SetBaud(uart, 115200);
	UART_SetFormat(uart, 8, UART_PARITY_NONE, 1);

	sei();

	char msg[] = "Hello World\n";
	while (true)
	{
		UART_Write(uart, msg, sizeof(msg));
		while (UART_IsBusy(uart));
	}

	return 0;
}
