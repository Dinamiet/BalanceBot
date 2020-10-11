#include "MPU6050.h"
#include "PID.h"
#include "Stepper.h"

#include <TimerOne.h>

#define CALIBRATE 0

#define GYRO_X_OFFSET	-50
#define GYRO_Y_OFFSET	-56
#define GYRO_Z_OFFSET	31

#define ACCEL_X_OFFSET	-7006
#define ACCEL_Y_OFFSET	4917
#define ACCEL_Z_OFFSET	8432


#define MPU_ADDRESS 0x68

#define INT_PIN			 2
#define STEPSPEED		 2000
#define HOLDING_TIME	 250
#define SWITCH_OFF_ANGLE 15

#define KP 30
#define KI 5
#define KD 1

#define BAUD_RATE 25 //running at 57600

MPU6050 imu(MPU_ADDRESS);
Stepper leftWheel(6, 7, 4, 5);
Stepper rightWheel(A2, A3, A0, A1);
PID		balanceController(KP, KI, KD);

volatile bool dataReady	 = false;
volatile bool stepMotors = false;

uint8_t leftWaiting	 = 0;
uint8_t rightWaiting = 0;
uint8_t waitForAngle = 0;

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

	delay(1000);

	pinMode(INT_PIN, INPUT);

	Serial.println(F("Initializing IMU..."));
	imu.initIMU();
#if CALIBRATE
	int16_t x, y, z;
	Serial.println(F("Calibrating Accelerometer..."));
	imu.CalibrateAccel(100, 25, 2);
	imu.getAccelOffset(&x, &y, &z);
	Serial.print(F("Accel offsets:\t"));
	Serial.print(x);
	Serial.print(F("\t"));
	Serial.print(y);
	Serial.print(F("\t"));
	Serial.println(z);

	Serial.println(F("Calibrating Gyro..."));
	imu.CalibrateGyro(100, 3);
	imu.getGyroOffset(&x, &y, &z);
	Serial.print(F("Gyro offsets:\t"));
	Serial.print(x);
	Serial.print(F("\t"));
	Serial.print(y);
	Serial.print(F("\t"));
	Serial.println(z);
#else
	Serial.println(F("Setting Gyro offsets"));
	imu.setGyroOffset(GYRO_X_OFFSET, GYRO_Y_OFFSET, GYRO_Z_OFFSET);
	Serial.println(F("Setting Accel offsets"));
	imu.setAccelOffset(ACCEL_X_OFFSET, ACCEL_Y_OFFSET, ACCEL_Z_OFFSET);
#endif

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

float roll;

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
			if (fabs(roll) > SWITCH_OFF_ANGLE * PI / 180)
			{
				leftWheel.setPos(0);
				rightWheel.setPos(0);
				balanceController.clearState();
				waitForAngle = roll > 0 ? 1 : 2;
			}

			waitForAngle = ((waitForAngle == 2 && roll > 0) || (waitForAngle == 1 && roll < 0)) ? 0 : waitForAngle;

			if (waitForAngle == 0)
			{
				float output = balanceController.Output(roll, 1);
				leftWheel.moveBy(output);
				rightWheel.moveBy(output);
			}
		}
		else
		{
			dataReady = false;
		}
	}
}
