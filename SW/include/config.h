#ifndef _CONFIG_H_
#define _CONFIG_H_

#include <stdbool.h>
#include <stddef.h>

void Setup_Config();
void Heartbeat_Enabled(bool enable);
void Config_SendDebug(void* data, size_t size);

#endif
