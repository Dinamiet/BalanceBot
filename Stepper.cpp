#include "Stepper.h"

Stepper::Stepper(uint8_t pinA0, uint8_t pinA1, uint8_t pinB0, uint8_t pinB1) : A_0(pinA0), A_1(pinA1), B_0(pinB0), B_1(pinB1) {}

void Stepper::init()
{
	pinMode(A_0, OUTPUT);
	pinMode(A_1, OUTPUT);
	pinMode(B_0, OUTPUT);
	pinMode(B_1, OUTPUT);

	position	= 0;
	desired_pos = 0;

	disable();
}

void Stepper::setPins(uint8_t value)
{
	digitalWrite(A_0, (value >> 3) & 0x1);
	digitalWrite(A_1, (value >> 2) & 0x1);
	digitalWrite(B_0, (value >> 1) & 0x1);
	digitalWrite(B_1, (value >> 0) & 0x1);
}

void Stepper::moveBy(int16_t steps) { desired_pos += steps; }

void Stepper::goTo(int16_t pos) { desired_pos = pos; }

bool Stepper::isEnabled() { return enabled; }

void Stepper::enable()
{
	enabled = true;
	setPins(position % STEP_SEQUENCE_LENGTH);
}

void Stepper::disable()
{
	enabled = false;
	setPins(0);
}

bool Stepper::isMoving() { return !(desired_pos == position); }

void Stepper::step()
{
	if (!isEnabled())
		enable();

	if (position > desired_pos)
	{
		position -= 1;
		setPins(position % STEP_SEQUENCE_LENGTH);
		return;
	}
	else if (position < desired_pos)
	{
		position += 1;
		setPins(position % STEP_SEQUENCE_LENGTH);
		return;
	}
}
