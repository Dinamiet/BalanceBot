
#include "cmdlist.h"

#include "cli.h"
#include "heartbeat.h"
#include "imu.h"
#include "motors.h"
#include "serial.h"
#include "task_scheduler.h"

#include <string.h>

extern TaskList taskList;

CLI cli;

#define CLI_CMD(cmd)                   \
	{                                  \
#cmd, Cmd_##cmd, CmdHelp_##cmd \
	}

static CLICommand cli_commands[] = {
		CLI_CMD(imu_req),
		CLI_CMD(cal_gyro),
		CLI_CMD(cal_accel),
		CLI_CMD(move),
		CLI_CMD(speed),
		CLI_CMD(hold),
		CLI_CMD(heartbeat),
		CLI_CMD(welcome),
		{"help", CLI_Cmd, CLI_Help},
		{0, 0, 0},
};

static size_t cli_read(char* str, size_t max) { return Serial_Read(Serial0, str, max); }

static size_t cli_write(char* str) { return Serial_Write(Serial0, str, strlen(str)); }

static void Task_CLI(void* cli) { CLI_ProcessCommand(cli, NULL); }

void Setup_CLI()
{
	CLI_Init(&cli, cli_commands, cli_read, cli_write);
	CLI_ProcessCommand(&cli, "welcome");
	TaskScheduler_CreateRetriggerTask(&taskList, "CLI", Task_CLI, &cli, CLI_PERIOD);
}

void Cmd_welcome(CLI* cli, int argc, char* argv[]) { cli->Write("\n-----------\nBalance Bot\n-----------\n"); }

char* CmdHelp_welcome[] = {
		"Welcomes user with a banner",
		"Usage: welcome",
		0,
};
