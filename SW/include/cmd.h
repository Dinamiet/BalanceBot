#ifndef _CMD_H_
#define _CMD_H_

#include "cli.h"
#include "stepper.h"

#include <stdlib.h>

extern Steppers steppers;

void Cmd_steppers(CLI* cli, int argc, char* argv[]);

extern char* CmdHelp_steppers[];

void Cmd_welcome(CLI* cli, int argc, char* argv[]);
extern char* CmdHelp_welcome[];

#endif
