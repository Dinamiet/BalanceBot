#include "cmds.h"

#include <stdlib.h>

void exit_Cmd(const CLI* cli, const size_t argc, char* const argv[])
{
	(void)cli;
	(void)argc;
	(void)argv;

	exit(0);
}

const char* const exit_Help[] = {
		"Exit Usage:",
		" Exit application",
		0,
};
