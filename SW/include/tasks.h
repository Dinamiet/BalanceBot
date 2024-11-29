#ifndef _TASKS_H_
#define _TASKS_H_

#define TASK_IMU_CONFIG       2
#define TASK_DELAY_IMU_CONFIG 100 // ms

#define TASK_IMU_INTERRUPT        3
#define TASK_IMU_PACKET_AVAILABLE 4
#define TASK_IMU_PACKET_READY     5

#define TASK_MOTOR_STEP            6
#define TASK_MOTORS_COOLDOWN       7
#define TASK_MOTOR_STEP_PERIOD     0   // ms
#define TASK_MOTORS_COOLDOWN_DELAY 200 // ms

#define TASK_DELAY_CONTROL      8
#define TASK_DELAY_CONTROL_TIME 1000 // ms

#define TASK_HEARTBEAT        9
#define TASK_HEARTBEAT_PERIOD 500 // ms

#define TASK_CONFIG_RECEIVE        10
#define TASK_CONFIG_RECEIVE_PERIOD 10 // ms

#define TASK_STEP_PIN_CLEAR 11

#endif
