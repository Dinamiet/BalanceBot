#ifndef _TASKS_H_
#define _TASKS_H_

#define TASK_CLI        1
#define TASK_PERIOD_CLI 50 // ms

#define TASK_IMU_CONFIG       2
#define TASK_DELAY_IMU_CONFIG 100 // ms

#define TASK_IMU_INTERRUPT        3
#define TASK_IMU_PACKET_AVAILABLE 4
#define TASK_IMU_PACKET_READY     5

#define TASK_MOTOR_STEP            6
#define TASK_MOTORS_COOLDOWN       7
#define TASK_MOTOR_STEP_PERIOD     200  // ms
#define TASK_MOTORS_COOLDOWN_DELAY 5000 // ms

#endif
