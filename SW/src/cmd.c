#include "cmd.h"

void Cmd_steppers(CLI* cli, int argc, char* argv[])
{
	cli->Write(argv[1]);
	cli->Write("\n");
	int16_t value = atoi(argv[1]);
	Steppers_MoveBy(&steppers, value);
}

char* CmdHelp_steppers[]= {
	"Stepper Interactions/Settings",
	"Usage: stepper [steps]",
	"Move stepper by specified steps",
	0
	};

void Cmd_welcome(CLI* cli, int argc, char* argv[]) {
	cli->Write("\n---------------------------\n\tBalance Bot\n---------------------------\n");
}

char* CmdHelp_welcome[] =
{
	"Welcomes user with a banner",
	"Usage: welcome",
	0
};
