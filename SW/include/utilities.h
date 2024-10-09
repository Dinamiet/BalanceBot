#ifndef _UTILITIES_H_
#define _UTILITIES_H_

#include "cli.h"
#include "scheduler.h"

extern CLI*       cmdLine;
extern Scheduler* taskScheduler;

void Setup_Scheduler();
void Setup_CLI();

#endif
