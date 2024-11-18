#include "cmds.h"
#include "drivers.h"

#include <stdlib.h>

void Cmd_scan(const CLI* cli, const size_t argc, const char* argv[])
{
	(void)argc;
	(void)argv;

	CLI_Write(cli, "Scanning...\n\r");
	CLI_Write(cli, "Response from: ");

	for (uint8_t address = 1; address < 127; address++)
	{
		I2CDevice tmpDevice = I2C_BindDevice(NULL, i2c, address, I2C_ADDRESSING_8BIT);
		I2C_Write(&tmpDevice, NULL, 0, NULL);
		while (I2C_IsBusy(&tmpDevice));
		if (I2C_GetStatus(&tmpDevice) == I2C_STATUS_OK)
		{
			CLI_Write(cli, "0x%02X \n\r", address);
		}
	}
	CLI_Write(cli, "\n\r");
}

const char* CmdHelp_scan[] = {
		"Scan I2C bus for devices",
		0,
};
