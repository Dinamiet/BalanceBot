#include "IMU.h"
#include "helper_3dmath.h"

#define MPU_ADDRESS 0x68
#define LED			13
#define INT_PIN		2
bool blinkState = false;

IMU			imu(MPU_ADDRESS);
Quaternion	q;
VectorFloat gravity;
float		ypr[3];

uint8_t GetYawPitchRoll(float *data, Quaternion *q, VectorFloat *gravity)
{
	// yaw: (about Z axis)
	data[0] = atan2(2 * q->x * q->y - 2 * q->w * q->z, 2 * q->w * q->w + 2 * q->x * q->x - 1);
	// pitch: (nose up/down, about Y axis)
	data[1] = atan2(gravity->x, sqrt(gravity->y * gravity->y + gravity->z * gravity->z));
	// roll: (tilt left/right, about X axis)
	data[2] = atan2(gravity->y, gravity->z);
	if (gravity->z < 0)
	{
		if (data[1] > 0)
		{
			data[1] = PI - data[1];
		}
		else
		{
			data[1] = -PI - data[1];
		}
	}
	return 0;
}

uint8_t GetGravity(VectorFloat *v, Quaternion *q)
{
	v->x = 2 * (q->x * q->z - q->w * q->y);
	v->y = 2 * (q->w * q->x + q->y * q->z);
	v->z = q->w * q->w - q->x * q->x - q->y * q->y + q->z * q->z;
	return 0;
}

uint8_t GetQuaternion(int16_t *data, const uint8_t *packet)
{
	// TODO: accommodate different arrangements of sent data (ONLY default supported now)
	data[0] = ((packet[0] << 8) | packet[1]);
	data[1] = ((packet[4] << 8) | packet[5]);
	data[2] = ((packet[8] << 8) | packet[9]);
	data[3] = ((packet[12] << 8) | packet[13]);
	return 0;
}

uint8_t GetQuaternion(Quaternion *q, const uint8_t *packet)
{
	// TODO: accommodate different arrangements of sent data (ONLY default supported now)
	int16_t qI[4];
	uint8_t status = GetQuaternion(qI, packet);
	if (status == 0)
	{
		q->w = (float)qI[0] / 16384.0f;
		q->x = (float)qI[1] / 16384.0f;
		q->y = (float)qI[2] / 16384.0f;
		q->z = (float)qI[3] / 16384.0f;
		return 0;
	}
	return status; // int16 return value, indicates error if this line is reached
}

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
		uint8_t	 data[DMP_PACKET_SIZE];
		if (fifoCount >= DMP_PACKET_SIZE)
		{
			//Do this math in MPU class
			imu.getFIFOBytes(data, DMP_PACKET_SIZE);
			GetQuaternion(&q, data);
			GetGravity(&gravity, &q);
			GetYawPitchRoll(ypr, &q, &gravity);
			Serial.print("ypr\t");
			Serial.print(fifoCount);
			Serial.print("\t");
			Serial.print(ypr[0] * 180 / M_PI);
			Serial.print("\t");
			Serial.print(ypr[1] * 180 / M_PI);
			Serial.print("\t");
			Serial.print(ypr[2] * 180 / M_PI);
			Serial.println();
			dataReady = false;
		}
		digitalWrite(LED, blinkState);
		blinkState = !blinkState;
	}

	//Handle FIFO overflow
}
