#ifndef _SETUP_H_
#define _SETUP_H_

void Setup_Serial();
void Setup_I2C();
void Setup_SystemTime();
void Setup_LED();
void Setup_Steppers();

void Setup_CLI();
void Setup_TaskScheduler();
void Setup_IMU();

void Create_Tasks();

#endif
