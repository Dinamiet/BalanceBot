#ifndef _UTILITIES_H_
#define _UTILITIES_H_

#include "cli.h"
#include "observer.h"
#include "scheduler.h"

extern CLI*       cmdLine;
extern Scheduler* taskScheduler;
extern Observer*  notifier;

void Setup_Scheduler();
void Setup_CLI();
void Setup_Notifier();

#endif
