#include "cmdlist.h"
#include "heartbeat.h"
#include "imu.h"
#include "motors.h"
#include "system.h"
#include "task_scheduler.h"

#include <avr/interrupt.h>
#include <stdbool.h>

extern TaskList taskList;

int main()
{
	Setup_Serial();
	Setup_I2C();
	Setup_SystemTime();
	Setup_TaskScheduler();

	sei();

	Setup_Heartbeat();
	Setup_CLI();

	Setup_Motors();
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
