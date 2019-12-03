#include "PID.h"

PID::PID(float P, float I, float D)
{
	Kp			  = P;
	Ki			  = I;
	Kd			  = D;
	Integral	  = 0;
	LastError	  = 0;
	preventWindup = false;
}

float PID::Output(float input, float dt)
{
	float error = sp - input;

	Integral += error * dt;
	if (preventWindup)
	{
		if (Integral > maxAllowedWindup)
		{
			Integral = maxAllowedWindup;
		}
		else if (Integral < -maxAllowedWindup)
		{
			Integral = -maxAllowedWindup;
		}
	}

	float P = Kp * error;
	float I = Ki * Integral;
	float D = Kd * (error - LastError) * dt;

	LastError = error;

	return P + I + D;
}

float PID::getKp() { return Kp; }
float PID::getKi() { return Ki; }
float PID::getKd() { return Kd; }

void PID::setKp(float P) { Kp = P; }
void PID::setKi(float I) { Ki = I; }
void PID::setKd(float D) { Kd = D; }

void PID::setPoint(float value) { sp = value; }

float PID::setPoint() { return sp; }

void PID::disableWindupPrevention() { preventWindup = false; }

void PID::enableWindupPrevention(float max)
{
	preventWindup	 = true;
	maxAllowedWindup = (max >= 0) ? max : -max;
}

float PID::getLastError() { return LastError; }
