#ifndef _CMD_H_
#define _CMD_H_

#include "cli.h"

#define DEFINE_CMD(cmd) \
void		 Cmd_##cmd(CLI* cli, int argc, char* argv[]); \
extern char* CmdHelp_##cmd[]

DEFINE_CMD(move);
DEFINE_CMD(speed);
DEFINE_CMD(hold);
DEFINE_CMD(led);
DEFINE_CMD(welcome);

#endif
