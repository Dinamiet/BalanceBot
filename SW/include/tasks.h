#ifndef _TASK_H_
#define _TASK_H_

#define CLI_TASK_NAME "CLI"
#define LED_TASK_NAME "LED"
#define STEPPER_TASK_NAME "Stepper Step"
#define STEPPER_HOLD_TASK_NAME "Stepper Disable"

#define CLI_PERIOD	 50	 // ms
#define LED_PERIOD	 500 // ms
#define STEP_SPEED	 2	 // ms
#define STEPPER_HOLD 250 // ms

void Task_CLI(void*);
void Task_LED(void*);
void Task_Stepper(void*);
void Task_StepperDisable(void*);

void IMU_DataReady_ISR();

#endif
