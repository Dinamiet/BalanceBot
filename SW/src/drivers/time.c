#include "drivers.h"

#define TIMER_COUNT_TO_MS        ((1024.0f * 1000.0f) / F_CPU)
#define TIME_ROLLOVER_CONVERSION (TIMER_COUNT_TO_MS * 256.0f)

Timer*          systemTimer;
static uint32_t rollOverCount;

static void rollOver();

static void rollOver() { ++rollOverCount; }

void Setup_SystemTime()
{
	rollOverCount = 0;
	systemTimer   = Timer_GetInstance(TIMER0);
	Timer_Init(systemTimer);
	Timer_Configure(systemTimer, TIMER_DIV_1024);
	Timer_EnableInterrupt(systemTimer, TIMER_INTERRUPT_ROLLOVER, rollOver);
	Timer_Start(systemTimer);
}

uint32_t System_GetTime()
{
	float ms = TIME_ROLLOVER_CONVERSION * rollOverCount;
	ms += TIMER_COUNT_TO_MS * Timer_GetValue(systemTimer);

	return ms;
}
