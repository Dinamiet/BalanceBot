#include "gpio.h"
#include "setup.h"
#include "task_scheduler.h"

#include <stdlib.h>

#define HEARTBEAT_TASK_NAME "Heartbeat"

extern TaskList taskList;

static void Task_Heartbeat(void* _)
{
	static uint8_t value = 0;
	GPIO_WritePin(GPIOB, 5, value++ & 0x01);
}

void Setup_Heartbeat()
{
	GPIO_PinMode(GPIOB, 5, GPIO_OUTPUT);
	TaskScheduler_CreateRetriggerTask(&taskList, HEARTBEAT_TASK_NAME, Task_Heartbeat, NULL, HEARTBEAT_PERIOD);
}

void Cmd_heartbeat(CLI* cli, int argc, char* argv[])
{
	uint32_t time = HEARTBEAT_PERIOD;
	if (argc > 1)
		time = atoi(argv[1]);

	Task* ledTask = TaskScheduler_FindTask(&taskList, HEARTBEAT_TASK_NAME);
	if (ledTask)
	{
		cli->Write("Setting LED period: ");
		cli->Write(argv[1]);
		cli->Write(" ms\n");
		TaskScheduler_ChangeTaskPeriod(ledTask, time);
	}
	else
	{
		cli->Write("Error occured\n");
	}
}

char* CmdHelp_heartbeat[] = {
		"Set Heartbeat flash period",
		"Usage: heartbeat [time]",
		"time - ms",
		0,
};
