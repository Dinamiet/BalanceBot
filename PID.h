#ifndef PID_H
#define PID_H

#include <avr/pgmspace.h>

class PID
{
	private:
	float Kp;
	float Ki;
	float Kd;
	float sp;
	float Integral;
	float LastError;
	bool  preventWindup;
	float maxAllowedWindup;

	public:
	PID(float P, float I, float D);

	float getKp();
	float getKi();
	float getKd();

	void setKp(float P);
	void setKi(float I);
	void setKd(float D);

	void  setPoint(float value);
	float setPoint();

	void disableWindupPrevention();
	void enableWindupPrevention(float max);

	float Output(float input, float dt);
	float getLastError();
};

#endif
