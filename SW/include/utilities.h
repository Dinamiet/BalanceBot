#ifndef _UTILITIES_H_
#define _UTILITIES_H_

#include "observer.h"
#include "scheduler.h"

extern Scheduler* taskScheduler;
extern Observer*  notifier;

void Setup_Scheduler();
void Setup_Notifier();

#endif
