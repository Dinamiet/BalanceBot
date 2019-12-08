#ifndef STEPPER_H
#define STEPPER_H

#define STEP_SEQUENCE_LENGTH 4

#include "Arduino.h"

#include <stdint.h>

class Stepper
{
	private:
	const uint8_t A_0, A_1, B_0, B_1;
	bool		  enabled;
	int16_t		  position;
	int16_t		  desired_pos;

	void setPins(uint8_t value);

	public:
	Stepper(uint8_t pinA0, uint8_t pinA1, uint8_t pinB0, uint8_t pinB1);

	void init();
	void moveBy(int16_t steps);
	void goTo(int16_t pos);
	bool isEnabled();
	void enable();
	void disable();
	void step();
	bool isMoving();
};

#endif
