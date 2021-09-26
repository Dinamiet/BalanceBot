#include "setup.h"

#include "cli.h"
#include "serial.h"

#include <string.h>

/*---------------------------------------------------------------- */
/* Serial Interface setup */
/*---------------------------------------------------------------- */
#define SERIAL_BUFFER_SIZE 128
uint8_t TXBuff_serial[SERIAL_BUFFER_SIZE];
uint8_t RXBuff_serial[SERIAL_BUFFER_SIZE/4];

#define SERIAL_BAUD 115200
void Setup_Serial()
{
	Serial_Configure(Serial0, SERIAL_8N1, SERIAL_BAUD);
	Serial_InitTXBuffer(Serial0, TXBuff_serial, sizeof(TXBuff_serial));
	Serial_InitRXBuffer(Serial0, RXBuff_serial, sizeof(RXBuff_serial));
	Serial_Enable(Serial0);
}

/*---------------------------------------------------------------- */
/* CLI Interface setup */
/*---------------------------------------------------------------- */
CLI		   cli;
CLICommand cli_commands[] = {{"help", CLI_Cmd, CLI_Help}, {0, 0, 0}};

size_t cli_read(char* str, size_t max) { return Serial_Read(Serial0, str, max); }

size_t cli_write(char* str) { return Serial_Write(Serial0, str, strlen(str)); }

void Setup_CLI()
{
	CLI_Init(&cli, cli_commands, cli_read, cli_write);
	CLI_ProcessCommand(&cli, "help");
}
