#include "tasks.h"

#include "cli.h"
#include "gpio.h"
#include "stepper.h"
#include "mpu6050.h"

void Task_CLI(void* _cli)
{
	CLI* cli = _cli;
	CLI_ProcessCommand(cli, NULL);
}

void Task_LED(void* _)
{
	static uint8_t value = 0;
	GPIO_WritePin(GPIOB, 5, value++ & 0x01);
}

void Task_Stepper(void* _stepper)
{
	Steppers* steppers = _stepper;
	Steppers_Step(steppers);
}

void Task_StepperDisable(void* _stepper)
{
	Steppers*	steppers	   = _stepper;
	static bool prevStationary = false;

	if (Steppers_IsEnabled(steppers))
	{
		bool stationary = !Steppers_IsMoving(steppers);
		if (stationary && prevStationary)
		{
			Steppers_Disable(steppers);
		}

		prevStationary = stationary;
	}
	else
	{
		prevStationary = false;
	}
}

extern MPU6050 imu;

void IMU_PacketRead(void* data)
{
	DMPPacket* packet= data;
}

void IMU_DataReady_ISR()
{
	MPU6050_RequestPacket(&imu, IMU_PacketRead);
}
