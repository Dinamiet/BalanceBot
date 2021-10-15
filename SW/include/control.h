#ifndef _CONTROL_H_
#define _CONTROL_H_

#include "setup.h"

void Setup_Control();

DEFINE_CMD(control_p);
DEFINE_CMD(control_i);
DEFINE_CMD(control_d);
DEFINE_CMD(sub);
DEFINE_CMD(unsub);

#endif
