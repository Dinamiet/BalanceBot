#include "MPU6050.h"
#include "PID.h"
#include "Stepper.h"

#include <TimerOne.h>

#define MPU_ADDRESS 0x68

#define INT_PIN			 2
#define STEPSPEED		 1000
#define HOLDING_TIME	 10
#define SWITCH_OFF_ANGLE 10

#define KP 0.005
#define KI 0.0001
#define KD 0

#define BAUD_RATE 25 //running at 57600

MPU6050 imu(MPU_ADDRESS);
Stepper leftWheel(6, 7, 4, 5);
Stepper rightWheel(A2, A3, A0, A1);
PID		balanceController(KP, KI, KD);

volatile bool dataReady	 = false;
volatile bool stepMotors = false;

uint8_t leftWaiting	 = 0;
uint8_t rightWaiting = 0;

void dmpDataReady() { dataReady = true; }

void stepMotorsCallback()
{
	leftWheel.step();
	rightWheel.step();

	if (leftWheel.isMoving())
	{
		leftWaiting = 0;
	}
	else if (leftWaiting++ > HOLDING_TIME)
	{
		leftWheel.disable();
	}

	if (rightWheel.isMoving())
	{
		rightWaiting = 0;
	}
	else if (rightWaiting++ > HOLDING_TIME)
	{
		rightWheel.disable();
	}

	stepMotors = true;
}

void setup()
{
	// put your setup code here, to run once:
	Serial.begin(115200);
	UBRR0H = BAUD_RATE >> 8;
	UBRR0L = BAUD_RATE;

	pinMode(INT_PIN, INPUT);

	Serial.println(F("Initializing IMU..."));
	imu.initIMU();

	Serial.println(F("Setting Accel offsets"));
	imu.setAccelOffset(-6968, 4939, 8436);
	// int16_t x, y, z;
	// Serial.println(F("Calibrating Accelerometer..."));
	// imu.CalibrateAccel(100, 25, 2);
	// imu.getAccelOffset(&x, &y, &z);
	// Serial.print(F("Accel offsets:\t"));
	// Serial.print(x);
	// Serial.print(F("\t"));
	// Serial.print(y);
	// Serial.print(F("\t"));
	// Serial.println(z);

	Serial.println(F("Setting Gyro offsets"));
	imu.setGyroOffset(-46, -52, 34);
	// Serial.println(F("Calibrating Gyro..."));
	// imu.CalibrateGyro(100, 3);
	// imu.getGyroOffset(&x, &y, &z);
	// Serial.print(F("Gyro offsets:\t"));
	// Serial.print(x);
	// Serial.print(F("\t"));
	// Serial.print(y);
	// Serial.print(F("\t"));
	// Serial.println(z);

	Serial.println(F("Initializing DMP..."));
	imu.initDMP();

	attachInterrupt(digitalPinToInterrupt(INT_PIN), dmpDataReady, RISING);

	Serial.println(F("Initializing motors"));
	leftWheel.init();
	rightWheel.init();

	Timer1.initialize(STEPSPEED);
	Timer1.attachInterrupt(stepMotorsCallback);

	Serial.println(F("Initialization Done!"));
	imu.INT_status();

	balanceController.setPoint(0);
}

uint16_t prevTime = 0;
float	 roll;

void loop()
{
	// put your main code here, to run repeatedly
	if (dataReady)
	{
		uint16_t packetCount = imu.numAvailablePackets();
		if (packetCount)
		{
			float yaw, pitch;
			imu.getYawPitchRoll(&yaw, &pitch, &roll);
		}
		else
		{
			dataReady = false;
		}
	}

	if (stepMotors)
	{
		//For in case we fall over we stop everything
		if (fabs(roll) > (SWITCH_OFF_ANGLE * PI / 180))
		{
			leftWheel.setPos(0);
			rightWheel.setPos(0);
		}
		else
		{
			uint16_t currTime = micros();
			float	 output	  = balanceController.Output(roll, prevTime - currTime);
			prevTime		  = currTime;

			leftWheel.goTo(output);
			rightWheel.goTo(output);
		}
	}
}
