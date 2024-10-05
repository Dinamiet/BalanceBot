#include "cmds.h"
#include "drivers.h"
#include "utilities.h"

#include <stdio.h>

#define PROMPT            "=>"
#define FORMATED_OUT_SIZE 32

#define CMD(x) {#x, Cmd_##x, CmdHelp_##x}

CLI* cli;

static CLI cmdLine;

static CLICommand commands[] = {
		CMD(uptime),
		{"?", CLI_Cmd, CLI_Help},
		{  0,       0,        0}
};

static size_t cliRead(char* str, const size_t max);
static size_t cliWrite(const char* str, va_list params);

static size_t cliRead(char* str, const size_t max) { return UART_Read(uart, str, max); }

static size_t cliWrite(const char* str, va_list params)
{
	char   cliBuff[FORMATED_OUT_SIZE];
	int    size    = vsnprintf(cliBuff, FORMATED_OUT_SIZE, str, params);
	size_t written = UART_Write(uart, cliBuff, size);
	while (UART_IsBusy(uart));
	return written;
}

void Setup_CLI()
{
	cli = &cmdLine;
	CLI_Init(cli, PROMPT, commands, cliRead, cliWrite);
}
