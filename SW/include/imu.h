#ifndef _IMU_H_
#define _IMU_H_

#include "drivers.h"
#include "mpu6050.h"

extern MPU* imu;
extern I2CDevice imuDevice;

void Setup_IMU();

#endif
