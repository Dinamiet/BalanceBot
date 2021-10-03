#include "cmd.h"

void Cmd_steppers(CLI* cli, int argc, char* argv[])
{
	cli->Write(argv[1]);
	cli->Write("\n");
	int16_t value = atoi(argv[1]);
	Steppers_MoveBy(&steppers, value);
}

void CmdHelp_steppers(CLI* cli) { cli->Write("Move stepper by specifed amount\n"); }

void Cmd_welcome(CLI* cli, int argc, char* argv[]) { CmdHelp_welcome(cli); }

void CmdHelp_welcome(CLI* cli)
{
	char* welcomeStr = "\n---------------------------\n\tBalance Bot\n---------------------------\n";

	cli->Write(welcomeStr);
	cli->Write("\n");
}
