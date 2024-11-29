#include "cmdLine.h"
#include "messages.h"
#include "structures.h"

void offset_handler(const DataPacket* dp, const void* data, const size_t size)
{
	(void)dp;
	(void)size;

	const OffsetValues* offset = data;

	CLI_Write(&cmdLine, "\rOffset:");
	CLI_Write(&cmdLine, ANSI_COLOR_FG(COLOR_RED) " %.6d", offset->X);
	CLI_Write(&cmdLine, ANSI_COLOR_FG(COLOR_GREEN) " %.6d", offset->Y);
	CLI_Write(&cmdLine, ANSI_COLOR_FG(COLOR_BLUE) " %.6d", offset->Z);
	CLI_Write(&cmdLine, ANSI_COLOR_FG(COLOR_DEFAULT) "\n\r");
}
