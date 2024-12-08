#include "private.h"
#include "serialport.h"

size_t Serial_Read(void* data, const size_t size) { return read(serialPort, data, size); }

size_t Serial_Write(const void* data, const size_t size) { return write(serialPort, data, size); }
