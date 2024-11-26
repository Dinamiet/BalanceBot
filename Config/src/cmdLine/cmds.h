#ifndef _CMDS_H_
#define _CMDS_H_

#include "cli.h"

void                     imu_Cmd(const CLI* cli, const size_t argc, char* const argv[]);
extern const char* const imu_Help[];

#endif
