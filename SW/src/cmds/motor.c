#include "cmds.h"
#include "motors.h"

void Cmd_motor(const CLI* cli, const size_t argc, const char* argv[])
{
	(void)argc;
	(void)argv;

	CLI_Write(cli, "Move by 10 steps\n\r");
	Motors_MoveBy(100);
}

const char* CmdHelp_motor[] = {
		"Move motor",
		0,
};
