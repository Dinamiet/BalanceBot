#include "gpio.h"

void Setup_GPIO()
{
	GPIO* gpio = GPIO_GetInstance(GPIO_D);
	GPIO_SetModePin(gpio, 2, GPIO_MODE_INPUT);
}
