#ifndef _CMDS_H_
#define _CMDS_H_

#include "cli.h"

void               Cmd_uptime(const CLI* cli, const size_t argc, const char* argv[]);
extern const char* CmdHelp_uptime[];

void               Cmd_scan(const CLI* cli, const size_t argc, const char* argv[]);
extern const char* CmdHelp_scan[];

#endif
