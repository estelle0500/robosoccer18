#include "Arduino.h"
#include "hardware_config.h"

Motor::Motor(byte enablePin, byte disablePin, byte rightPWMPin, byte leftPWMPin, boolean dirSwitch)
{
	_enablePin = enablePin;
	_disablePin = disablePin;
	_dirSwitch = dirSwitch;
	if (_dirSwitch == false){
		_rightPWMPin = rightPWMPin;
		_leftPWMPin = leftPWMPin;
	}
	else{
		_rightPWMPin = leftPWMPin;
		_leftPWMPin = rightPWMPin;
	}
	pinMode(_enablePin,OUTPUT);
	pinMode(_disablePin,OUTPUT);
	pinMode(_rightPWMPin,OUTPUT);
	pinMode(_leftPWMPin,OUTPUT);
}

void Motor::move(int motorSpeed)
{
	_motorSpeed = motorSpeed;
	if ( _motorSpeed < 0 )
	{
		_motorSpeed = -_motorSpeed;
		_motorDirection=0;
	}
	else if ( _motorSpeed ==0) _motorDirection=1;
	else _motorDirection=2;
	//Serial.println(_motorDirection);
	switch(_motorDirection)
	{
		case 0:
			digitalWrite(_enablePin,HIGH);
			digitalWrite(_disablePin,LOW);
			analogWrite(_leftPWMPin,HIGH);
			analogWrite(_rightPWMPin,_motorSpeed);
			break;
		case 1:
			digitalWrite(_enablePin,HIGH);
			digitalWrite(_disablePin,LOW);
			analogWrite(_leftPWMPin,HIGH);
			analogWrite(_rightPWMPin,HIGH);
			break;
		case 2:
			digitalWrite(_enablePin,HIGH);
			digitalWrite(_disablePin,LOW);
			analogWrite(_leftPWMPin,_motorSpeed);
			analogWrite(_rightPWMPin,HIGH);
			break;
	}
}

void Motor::coast()
{
	digitalWrite(_enablePin,LOW);
	digitalWrite(_disablePin,LOW);
	digitalWrite(_leftPWMPin,HIGH);
	digitalWrite(_rightPWMPin,HIGH);
}
