#include "drivers.h"

#define UART_BUFFER_SIZE 128

UART* uart;

static uint8_t uartTxBuff[UART_BUFFER_SIZE];
static uint8_t uartRxBuff[UART_BUFFER_SIZE];

void Setup_UART()
{
	uart = UART_GetInstance(UART0);

	UART_Init(uart);
	UART_AssignReceiveBuffer(uart, uartRxBuff, sizeof(uartRxBuff));
	UART_AssignTransmitBuffer(uart, uartTxBuff, sizeof(uartTxBuff));
	UART_SetBaud(uart, 9600);
	UART_SetFormat(uart, 8, UART_PARITY_NONE, 1);
}
