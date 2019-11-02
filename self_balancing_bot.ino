#include "Wire.h"

#define MPU_ADDRESS 0x68
#define ACCEL_START_REG 0x3B
#define GYRO_START_REG 0x43
#define TEMP_START_REG 0x41

#define LED 13
#define UPDATE_RATE 10
bool blinkState= false;

void setup() {
  // put your setup code here, to run once:
  Wire.begin();

  pinMode(LED, OUTPUT);
  digitalWrite(LED, blinkState);

  Serial.begin(115200);

  initIMU();
}

int16_t rawAccX, rawAccY, rawAccZ, rawTemp, rawGyroX, rawGyroY, rawGyroZ= 0;
float scaledAccX, scaledAccY, scaledAccZ, scaledTemp, scaledGyroX, scaledGyroY, scaledGyroZ= 0.0f;

void loop() {
  // put your main code here, to run repeatedly
  getAccel(&rawAccX, &rawAccY, &rawAccZ);

  getGyro(&rawGyroX, &rawGyroY, &rawGyroZ);

  getTemp(&rawTemp);
  //Scaling of values
  scaledAccX= rawAccX/ -16384.0f;
  scaledAccY= rawAccY/ -16384.0f;
  scaledAccZ= rawAccZ/ -16384.0f;

  scaledTemp= rawTemp / 340.0f + 36.53f;

  scaledGyroX= rawGyroX / 131.0f;
  scaledGyroY= rawGyroY / 131.0f;
  scaledGyroZ= rawGyroZ / 131.0f;

  //Remove offsets

  //Print values
  Serial.print(scaledAccX);
  Serial.print("\t");
  Serial.print(scaledAccY);
  Serial.print("\t");
  Serial.print(scaledAccZ);
  Serial.print("\t");
  Serial.print(scaledGyroX);
  Serial.print("\t");
  Serial.print(scaledGyroY);
  Serial.print("\t");
  Serial.print(scaledGyroZ);
  Serial.print("\t");
  Serial.print(scaledTemp);
  Serial.println();

  digitalWrite(LED, blinkState);
  blinkState= !blinkState;

  delay(UPDATE_RATE);
}


void initIMU()
{
  Wire.beginTransmission(MPU_ADDRESS);
  Wire.write(0x1C);
  Wire.write(0x00); //Accelerometer full scale range and selfTest
  Wire.endTransmission(true);

  Wire.beginTransmission(MPU_ADDRESS);
  Wire.write(0x1B);
  Wire.write(0x00); //Gyro full scale range and selfTest
  Wire.endTransmission(true);

  Wire.beginTransmission(MPU_ADDRESS);
  Wire.write(0x6B);
  Wire.write(0x00); //Sleep mode and CLK source
  Wire.endTransmission(true);
}

void getAccel(int16_t* x, int16_t* y, int16_t* z)
{
  Wire.beginTransmission(MPU_ADDRESS);
  Wire.write(ACCEL_START_REG);
  Wire.endTransmission(false);
  Wire.requestFrom(MPU_ADDRESS, 6);

  *x= (Wire.read() << 8) | Wire.read();
  *y= (Wire.read() << 8) | Wire.read();
  *z= (Wire.read() << 8) | Wire.read();
}

void getGyro(int16_t* x, int16_t* y, int16_t* z)
{
  Wire.beginTransmission(MPU_ADDRESS);
  Wire.write(GYRO_START_REG);
  Wire.endTransmission(false);
  Wire.requestFrom(MPU_ADDRESS, 6);

  *y= (Wire.read() << 8) | Wire.read();
  *x= (Wire.read() << 8) | Wire.read();
  *z= (Wire.read() << 8) | Wire.read();
}

void getTemp(int16_t* temp)
{
  Wire.beginTransmission(MPU_ADDRESS);
  Wire.write(TEMP_START_REG);
  Wire.endTransmission(false);
  Wire.requestFrom(MPU_ADDRESS, 2);

  *temp= (Wire.read() << 8) | Wire.read();
}
