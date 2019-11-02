#include "Wire.h"

#define MPU_ADDRESS 0x68
#define LED 13
bool blinkState= false;

void setup() {
  // put your setup code here, to run once:
  pinMode(LED, OUTPUT);
  digitalWrite(LED, blinkState);
  Serial.begin(115200);

  Wire.beginTransmission(MPU_ADDRESS);
  Wire.write(0x1C);
  Wire.write(0x00); //Accelerometer full scale range and selfTest
  Wire.endTransmission(true);

  Wire.beginTransmission(MPU_ADDRESS);
  Wire.write(0x1B);
  Wire.write(0x00); //Gyro full scale range and selfTest
  Wire.endTransmission(true);

  Wire.begin();
  Wire.beginTransmission(MPU_ADDRESS);
  Wire.write(0x6B);
  Wire.write(0x00); //Sleep mode and CLK source
  Wire.endTransmission(true);
}

int16_t rawAccX, rawAccY, rawAccZ, rawTemp, rawGyroX, rawGyroY, rawGyroZ= 0;
float scaledAccX, scaledAccY, scaledAccZ, scaledTemp, scaledGyroX, scaledGyroY, scaledGyroZ= 0.0f;

void loop() {
  // put your main code here, to run repeatedly

  Wire.beginTransmission(MPU_ADDRESS);
  Wire.write(0x3B);
  Wire.endTransmission(false);
  Wire.requestFrom(MPU_ADDRESS, 6);

  rawAccX= (Wire.read() << 8) | Wire.read();
  rawAccY= (Wire.read() << 8) | Wire.read();
  rawAccZ= (Wire.read() << 8) | Wire.read();

  Wire.beginTransmission(MPU_ADDRESS);
  Wire.write(0x41);
  Wire.endTransmission(false);
  Wire.requestFrom(MPU_ADDRESS, 2);

  rawTemp= (Wire.read() << 8) | Wire.read();

  Wire.beginTransmission(MPU_ADDRESS);
  Wire.write(0x43);
  Wire.endTransmission(false);
  Wire.requestFrom(MPU_ADDRESS, 6);

  rawGyroX= (Wire.read() << 8) | Wire.read();
  rawGyroY= (Wire.read() << 8) | Wire.read();
  rawGyroZ= (Wire.read() << 8) | Wire.read();

  //Scaling of values
  scaledAccX= rawAccX/16384.0f;
  scaledAccY= rawAccY/16384.0f;
  scaledAccZ= rawAccZ/16384.0f;

  scaledTemp= rawTemp / 340.0f + 36.53;

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
  Serial.println(scaledTemp);

  delay(100);
}
