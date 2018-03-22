#ifndef Dribbler_h
#define Dribbler_h

#include "Arduino.h"
#include "Motor.h"

class Dribbler{
  public:
	Motor leftDribbler, rightDribbler;
	Dribbler(int left[4], int right[4]);
	void on(int velocity);
        void off();
};

#endif	