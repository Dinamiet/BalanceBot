#include "cmdLine.h"
#include "messages.h"

void heartbeat_handler(const DataPacket* dp, const void* data, const size_t size)
{
	(void)dp;
	(void)data;
	(void)size;
	static uint8_t index      = 0;
	static char    sequence[] = {'|', '/', '-', '\\'};

	CLI_Write(&cmdLine, SCP "\r%c" RCP, sequence[index]);
	index = (index + 1) % sizeof(sequence);
}
