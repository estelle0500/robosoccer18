#ifndef MOTOR_H
#define MOTOR_H

#include "Arduino.h"

class Motor
{
public:
	Motor(byte enablePin, byte disablePin, byte rightPWMPin, byte leftPWMPin, boolean dirSwitch);
	void move(int motorSpeed);
	void coast();
private:
	byte _enablePin;
	byte _disablePin;
	byte _rightPWMPin;
	byte _leftPWMPin;
	boolean _dirSwitch;
	byte _motorDirection;
	int _motorSpeed;
};

#endif
