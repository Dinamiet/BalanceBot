#include "tasks.h"

#include "cli.h"
#include "gpio.h"

void Task_CLI(void* _cli)
{
	CLI* cli = _cli;
	CLI_ProcessCommand(cli, NULL);
}

void Task_LED(void* _)
{
	static uint8_t value = 0;
	GPIO_WritePin(GPIOB, 5, value++ & 0x01);
}
