#ifndef _IMU_H_
#define _IMU_H_

#include "setup.h"

void Setup_IMU();

DEFINE_CMD(imu_avail);
DEFINE_CMD(cal_gyro);
DEFINE_CMD(cal_accel);

#endif
