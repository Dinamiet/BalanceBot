#include "IMU.h"

#define MPU_ADDRESS 0x68
#define LED			13
#define UPDATE_RATE 10
bool blinkState = false;

IMU imu(MPU_ADDRESS);

void setup()
{
	// put your setup code here, to run once:
	pinMode(LED, OUTPUT);
	digitalWrite(LED, blinkState);

	imu.initIMU();

	Serial.begin(115200);
}

float AccX, AccY, AccZ, Temp, GyroX, GyroY, GyroZ = 0.0f;

void loop()
{
	// put your main code here, to run repeatedly
	imu.getAccel(&AccX, &AccY, &AccZ);
	imu.getGyro(&GyroX, &GyroY, &GyroZ);
	imu.getTemp(&Temp);
	//Remove offsets

	//Print values
	// Serial.print(AccX);
	// Serial.print("\t");
	// Serial.print(AccY);
	// Serial.print("\t");
	// Serial.print(AccZ);
	// Serial.print("\t");
	Serial.print(GyroX);
	Serial.print("\t");
	Serial.print(GyroY);
	Serial.print("\t");
	Serial.print(GyroZ);
	// Serial.print("\t");
	// Serial.print(Temp);
	Serial.println();

	digitalWrite(LED, blinkState);
	blinkState = !blinkState;

	delay(UPDATE_RATE);
}
