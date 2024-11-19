#ifndef _DRIVERS_H_
#define _DRIVERS_H_

#include "gpio.h"
#include "i2c.h"
#include "timer.h"
#include "uart.h"

extern UART*  uart;
extern I2C*   i2c;
extern Timer* systemTimer;

void Setup_UART();
void Setup_I2C();
void Setup_SystemTime();
void Setup_GPIO();

uint32_t System_GetTime();

#endif
