#include "cmdLine.h"
#include "messages.h"

void calibration_handler(const DataPacket* dp, const void* data, const size_t size)
{
	(void)dp;
	(void)data;
	(void)size;

	CLI_Write(&cmdLine, ANSI_COLOR_FG(COLOR_CYAN) "DONE\n" ANSI_COLOR_FG(DEFAULT_COLOR));
}
