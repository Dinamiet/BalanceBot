#ifndef _TCP_H_
#define _TCP_H_

#include <stddef.h>

void   TCP_Setup(const char* host);
size_t TCP_Read(void* data, const size_t size);
size_t TCP_Write(const void* data, const size_t size);

#endif
