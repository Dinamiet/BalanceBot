#ifndef _SERIAL_PORT_H_
#define _SERIAL_PORT_H_

#include <stddef.h>

void Serial_Setup(char* deviceFile);
size_t Serial_Read(void* data, const size_t size);
size_t Serial_Write(const void* data, const size_t size);

#endif
