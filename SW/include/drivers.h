#ifndef _DRIVERS_H_
#define _DRIVERS_H_

#include "timer.h"
#include "uart.h"

extern UART* uart;
extern Timer* systemTimer;

void Setup_UART();
void Setup_SystemTime();

uint32_t System_GetTime();

#endif
