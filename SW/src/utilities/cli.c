#include "cmds.h"
#include "drivers.h"
#include "tasks.h"
#include "utilities.h"

#include <stdio.h>

#define PROMPT            "=>"
#define FORMATED_OUT_SIZE 32

#define CMD(x) {#x, Cmd_##x, CmdHelp_##x}

CLI* cmdLine;

static CLI           cli;
static SchedulerTask cliTask;

static CLICommand commands[] = {
		CMD(uptime),
		CMD(scan),
		CMD(imu),
		CMD(motor),
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
	if (size > 0)
	{
		int sent = 0;
		while (sent < size) { sent += UART_Write(uart, &cliBuff[sent], size - sent); }
		return size;
	}
	return 0;
}

void Setup_CLI()
{
	cmdLine = &cli;
	CLI_Init(cmdLine, PROMPT, commands, cliRead, cliWrite);

	Scheduler_CreateRecurringTask(taskScheduler, &cliTask, TASK_CLI, (Scheduler_TaskFunction)CLI_Process, cmdLine, TASK_PERIOD_CLI);
}
