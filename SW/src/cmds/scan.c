#include "cmds.h"
#include "drivers.h"

#include <stdlib.h>

void Cmd_scan(const CLI* cli, const size_t argc, const char* argv[])
{
	(void)argc;
	(void)argv;

	for (uint8_t address = 1; address < 127; address++)
	{
		I2C_Write(i2c, address, NULL, 0, NULL, NULL);
		while (I2C_IsBusy(i2c));
		if (I2C_GetStatus(i2c) == I2C_STATUS_OK)
		{
			CLI_Write(cli, "Reply from 0x%02X\n\r", address);
		}
	}
}

const char* CmdHelp_scan[] = {
		"Scan I2C bus for devices",
		0,
};
