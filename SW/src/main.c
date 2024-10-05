#include "drivers.h"
#include "utilities.h"

#include <avr/interrupt.h>
#include <stdbool.h>
#include <stdint.h>

int main()
{
	Setup_SystemTime();
	Setup_UART();

	sei();

	Setup_Scheduler();
	Setup_CLI();

	SchedulerTask* task = NULL;
	while (true)
	{
		task = Scheduler_NextReady(taskScheduler);
		Scheduler_Execute(task);
		Scheduler_Queue(taskScheduler, task);
	}

	return 0;
}
