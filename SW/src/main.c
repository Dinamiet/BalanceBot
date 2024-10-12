#include "drivers.h"
#include "imu.h"
#include "utilities.h"

#include <avr/interrupt.h>
#include <stdbool.h>
#include <stdint.h>

int main()
{
	Setup_SystemTime();
	Setup_UART();
	Setup_I2C();
	Setup_GPIO();

	sei();

	Setup_Scheduler();
	Setup_CLI();

	Setup_IMU();

	CLI_Write(cmdLine, "\n\n\r\tWelcome\n\n\r");

	SchedulerTask* task = NULL;
	while (true)
	{
		task = Scheduler_NextReady(taskScheduler);
		Scheduler_Execute(task);
		Scheduler_Queue(taskScheduler, task);
	}

	return 0;
}
