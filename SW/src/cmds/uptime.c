#include "cmds.h"
#include "drivers.h"

void Cmd_uptime(const CLI* cli, const size_t argc, const char* argv[])
{
	(void)argc;
	(void)argv;

	uint32_t currentTime = System_GetTime();
	CLI_Write(cli, "Uptime: %u\n\r", currentTime);
}

const char* CmdHelp_uptime[] = {
		"Display uptime (ms)",
		0,
};
