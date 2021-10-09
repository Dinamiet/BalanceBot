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
	Setup_I2C();

	Setup_CLI();
	Setup_TaskScheduler();

	sei();

	Create_Tasks();

	Setup_Steppers();

	Setup_IMU();

	Task* activeTask = NULL;

	while (true)
	{
		activeTask = TaskScheduler_ReadyTask(&taskList);
		TaskScheduler_ExecuteTask(activeTask);
		TaskScheduler_QueueTask(&taskList, activeTask);
	}

	return 0;
}
