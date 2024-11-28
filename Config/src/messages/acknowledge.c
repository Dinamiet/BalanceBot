#include "cmdLine.h"
#include "messages.h"

void acknowledge_handler(const DataPacket* dp, const void* data, const size_t size)
{
	(void)dp;
	(void)size;
	(void)data;

	CLI_Write(&cmdLine, ANSI_COLOR_FG(GREEN) "OK\n" ANSI_COLOR_FG(COLOR_DEFAULT));
}
