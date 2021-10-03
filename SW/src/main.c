#include "setup.h"
#include "task_scheduler.h"

#include <avr/interrupt.h>
#include <stdbool.h>

extern TaskList taskList; // Defined in setup.c

int main()
{
	Setup_Serial();
	Setup_SystemTime();
	Setup_LED();
	Setup_Steppers();

	Setup_CLI();
	Setup_TaskScheduler();

	sei();

	Create_Tasks();

	Task* activeTask = NULL;

	while (true)
	{
		activeTask = TaskScheduler_ReadyTask(&taskList);
		TaskScheduler_ExecuteTask(activeTask);
		TaskScheduler_CleanTask(&taskList, activeTask);
	}

	return 0;
}
