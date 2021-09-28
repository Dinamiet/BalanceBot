#include "tasks.h"

#include "cli.h"

void Task_CLI(void* _cli)
{
	CLI* cli = _cli;
	CLI_ProcessCommand(cli, NULL);
}
