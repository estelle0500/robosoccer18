#ifndef hardware_config_h
#define hardware_config_h

#include "Arduino.h"

class Motor
{
public:
	Motor();
	void init(byte enablePin, byte disablePin, byte rightPWMPin, byte leftPWMPin, boolean dirSwitch);
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