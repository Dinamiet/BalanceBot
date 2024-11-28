#ifndef _SERIAL_PORT_H_
#define _SERIAL_PORT_H_

#include <fcntl.h>
#include <stdio.h>
#include <unistd.h>

extern int serialPort;

void Serial_Setup(char* deviceFile);

#endif
