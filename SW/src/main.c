#include "config.h"
#include "control.h"
#include "drivers.h"
#include "imu.h"
#include "motors.h"
#include "utilities.h"

#include <avr/interrupt.h>
#include <stdbool.h>
#include <stdint.h>

int main()
{
	Setup_SystemTime();
	Setup_UART();
	Setup_I2C();

	sei();

	Setup_Scheduler();
	Setup_Notifier();

	Setup_IMU();
	Setup_Motors();

	Setup_Control();

	Setup_Config();

	SchedulerTask* task = NULL;
	while (true)
	{
		task = Scheduler_NextReady(taskScheduler);
		Scheduler_Execute(task);
		Scheduler_Queue(taskScheduler, task);
	}

	return 0;
}
