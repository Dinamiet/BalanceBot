#include "MPU6050.h"
#include "Stepper.h"

#define MPU_ADDRESS 0x68
#define LED			13
#define INT_PIN		2
bool blinkState = false;

MPU6050 imu(MPU_ADDRESS);
Stepper leftWheel(1, 2, 3, 4);
Stepper rightWheel(5, 6, 7, 8);

volatile bool dataReady = false;

void dmpDataReady() { dataReady = true; }

void setup()
{
	// put your setup code here, to run once:
	Serial.begin(115200);

	pinMode(LED, OUTPUT);
	pinMode(INT_PIN, INPUT);
	digitalWrite(LED, blinkState);

	Serial.println(F("Initializing IMU..."));
	imu.initIMU();

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

	Serial.println(F("Initializing DMP..."));
	imu.initDMP();

	attachInterrupt(digitalPinToInterrupt(INT_PIN), dmpDataReady, RISING);

	Serial.println(F("Initialization Done!"));
	imu.INT_status();

	leftWheel.init();
	rightWheel.init();
}

float AccX, AccY, AccZ, Temp, GyroX, GyroY, GyroZ = 0.0f;

void loop()
{
	// put your main code here, to run repeatedly
	if (dataReady)
	{
		uint16_t packetCount = imu.numAvailablePackets();
		if (packetCount)
		{
			float yaw, pitch, roll;
			imu.getYawPitchRoll(&yaw, &pitch, &roll);
			Serial.print(F("ypr\t"));
			Serial.print(packetCount);
			Serial.print(F("\t"));
			Serial.print(yaw * 180 / M_PI);
			Serial.print(F("\t"));
			Serial.print(pitch * 180 / M_PI);
			Serial.print(F("\t"));
			Serial.print(roll * 180 / M_PI);
			Serial.println();
			dataReady = false;
			digitalWrite(LED, blinkState);
			blinkState = !blinkState;
		}
	}

	//Handle FIFO overflow
}
