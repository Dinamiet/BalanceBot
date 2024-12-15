#include "cmdLine.h"
#include "cmds.h"

#include <stdio.h>
#include <termios.h>
#include <unistd.h>

#define PROMPT "->"
#define CMD(x) {#x, x##_Cmd, x##_Help}

CLI cmdLine;
static struct termios original_settings;

static size_t cmdLine_read(char* str, size_t max);
static size_t cmdLine_write(const char* format, va_list params);

static size_t cmdLine_read(char* str, size_t max)
{
	int bytesRead = read(STDIN_FILENO, str, max);
	if (bytesRead < 0)
		return 0;

	return bytesRead;
}

static size_t cmdLine_write(const char* format, va_list params)
{
	int written = vprintf(format, params);
	fflush(stdout);
	return written;
}

CLICommand cmdList[] = {
		CMD(imu),
		CMD(gyro),
		CMD(accel),
		CMD(control),
		CMD(exit),
		CMD(motor),
		CMD(heartbeat),
		CMD(pos),
		{"help", CLI_Cmd, CLI_Help},
		{     0,       0,        0}
};

void CommandLine_Setup()
{
	struct termios ctrl;
	tcgetattr(STDIN_FILENO, &original_settings);

	cfmakeraw(&ctrl);
	ctrl.c_cc[VMIN]  = 1;
	ctrl.c_cc[VTIME] = 1;
	tcsetattr(STDIN_FILENO, TCSANOW, &ctrl);

	CLI_Init(&cmdLine, PROMPT, cmdList, cmdLine_read, cmdLine_write);
}

void CommandLine_Restore() { tcsetattr(STDIN_FILENO, TCSANOW, &original_settings); }
