#include "drivers.h"
#include "utilities.h"

Scheduler* taskScheduler;

static Scheduler scheduler;

void Setup_Scheduler()
{
	taskScheduler = &scheduler;
	Scheduler_Init(taskScheduler, System_GetTime);
}
