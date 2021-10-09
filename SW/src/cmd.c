#include "cmd.h"

#include "stepper.h"
#include "task_scheduler.h"
#include "tasks.h"

#include <stdlib.h>

extern Steppers steppers;
extern TaskList taskList;

void Cmd_move(CLI* cli, int argc, char* argv[])
{
	cli->Write("Move steppers: ");
	cli->Write(argv[1]);
	cli->Write(" steps\n");
	int16_t value = atoi(argv[1]);
	Steppers_MoveBy(&steppers, value);
}

char* CmdHelp_move[] = {
		"Move Stepper motors",
		"Usage: move [steps]",
		0,
};

void Cmd_speed(CLI* cli, int argc, char* argv[])
{
	uint32_t time		 = atoi(argv[1]);
	Task*	 stepperTask = TaskScheduler_FindTask(&taskList, STEPPER_TASK_NAME);
	if (stepperTask)
	{
		cli->Write("Setting time between steps: ");
		cli->Write(argv[1]);
		cli->Write(" ms\n");
		TaskScheduler_ChangeTaskPeriod(stepperTask, time);
	}
	else
	{
		cli->Write("Error occured\n");
	}
}

char* CmdHelp_speed[] = {
		"Set speed of stepper motors",
		"Usage: speed [time]",
		"time - ms/step",
		0,
};

void Cmd_hold(CLI* cli, int argc, char* argv[])
{
	uint32_t time	  = atoi(argv[1]);
	Task*	 holdTask = TaskScheduler_FindTask(&taskList, STEPPER_HOLD_TASK_NAME);
	if (holdTask)
	{
		cli->Write("Setting hold time: ");
		cli->Write(argv[1]);
		cli->Write(" ms\n");
		TaskScheduler_ChangeTaskPeriod(holdTask, time / 2);
	}
	else
	{
		cli->Write("Error occured\n");
	}
}

char* CmdHelp_hold[] = {
		"Set hold time of stepper motors",
		"Usage: hold [time]",
		"time - ms",
		0,
};

void Cmd_led(CLI* cli, int argc, char* argv[])
{
	uint32_t time	 = atoi(argv[1]);
	Task*	 ledTask = TaskScheduler_FindTask(&taskList, LED_TASK_NAME);
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

char* CmdHelp_led[] = {
		"Set LED flash period",
		"Usage: led [time]",
		"time - ms",
		0,
};

void Cmd_welcome(CLI* cli, int argc, char* argv[]) { cli->Write("\n-----------\nBalance Bot\n-----------\n"); }

char* CmdHelp_welcome[] = {
		"Welcomes user with a banner",
		"Usage: welcome",
		0,
};
