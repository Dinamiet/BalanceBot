#include "setup.h"
#include "steppers.h"
#include "task_scheduler.h"

#include <stdlib.h>

#define STEPPER_TASK_NAME	   "Stepper Step"
#define STEPPER_HOLD_TASK_NAME "Stepper Disable"

extern TaskList taskList;

static void Task_Stepper(void* steppers) { Steppers_Step(steppers); }

static void Task_StepperDisable(void* steppers)
{
	static bool prevStationary = false;

	if (Steppers_IsEnabled(steppers))
	{
		bool stationary = !Steppers_IsMoving(steppers);
		if (stationary && prevStationary)
		{
			Steppers_Disable(steppers);
		}

		prevStationary = stationary;
	}
	else
	{
		prevStationary = false;
	}
}

Steppers steppers;
void	 Setup_Motors()
{
	Steppers_Init(&steppers, GPIOD, GPIOC);
	TaskScheduler_CreateRetriggerTask(&taskList, STEPPER_TASK_NAME, Task_Stepper, &steppers, STEP_SPEED);
	TaskScheduler_CreateRetriggerTask(&taskList, STEPPER_HOLD_TASK_NAME, Task_StepperDisable, &steppers, STEPPER_HOLD / 2);
}

void Cmd_move(CLI* cli, int argc, char* argv[])
{
	cli->Write("Move steppers: ");
	cli->Write(argv[1]);
	cli->Write(" steps\n");
	int16_t value = atoi(argv[1]);
	Steppers_MoveBy(&steppers, value);
}

char* CmdHelp_move[] = {
		// "Move Stepper motors",
		// "Usage: move [steps]",
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
		// "Set speed of stepper motors",
		// "Usage: speed [time]",
		// "time - ms/step",
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
		// "Set hold time of stepper motors",
		// "Usage: hold [time]",
		// "time - ms",
		0,
};
