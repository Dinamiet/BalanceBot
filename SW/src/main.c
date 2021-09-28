#include "setup.h"
#include "task_scheduler.h"

#include <avr/interrupt.h>
#include <stdbool.h>

extern TaskList taskList; // Defined in setup.c

int main()
{
	Setup_Serial();
	Setup_SystemTime();
	Setup_CLI();
	Setup_TaskScheduler();

	sei();

	Create_Tasks();

	while (true) { TaskScheduler_RunNextTask(&taskList); }

	return 0;
}
