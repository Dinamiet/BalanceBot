#include "MPU6050.h"

#define MPU_ADDRESS 0x68
#define LED			13
#define INT_PIN		2
bool blinkState = false;

MPU6050 imu(MPU_ADDRESS);

volatile bool dataReady = false;

void dmpDataReady()
{
	dataReady = true;
	// Serial.println("INT");
}

void setup()
{
	// put your setup code here, to run once:
	Serial.begin(115200);

	pinMode(LED, OUTPUT);
	pinMode(INT_PIN, INPUT);
	digitalWrite(LED, blinkState);

	Serial.println("Initializing IMU...");
	imu.initIMU();

	// Calibration Code

	Serial.println("Initializing DMP...");
	imu.initDMP();

	Serial.println("Setting offesets...");
	imu.setGyroOffset(-48, -57, 36);
	imu.setAccelOffset(-6556, 4883, 8444);

	attachInterrupt(digitalPinToInterrupt(INT_PIN), dmpDataReady, RISING);

	Serial.println("Initialization Done!");
	imu.INT_status();
}

float AccX, AccY, AccZ, Temp, GyroX, GyroY, GyroZ = 0.0f;

void loop()
{
	// put your main code here, to run repeatedly
	if (dataReady)
	{
		uint16_t fifoCount = imu.getFIFOCount();
		float	 yaw, pitch, roll;
		if (fifoCount >= DMP_PACKET_SIZE)
		{
			//Do this math in MPU class
			imu.getYawPitchRoll(&yaw, &pitch, &roll);
			Serial.print("ypr\t");
			// Serial.print(fifoCount);
			// Serial.print("\t");
			Serial.print(yaw * 180 / M_PI);
			Serial.print("\t");
			Serial.print(pitch * 180 / M_PI);
			Serial.print("\t");
			Serial.print(roll * 180 / M_PI);
			Serial.println();
			dataReady = false;
		}
		digitalWrite(LED, blinkState);
		blinkState = !blinkState;
	}

	//Handle FIFO overflow
}
