#ifndef MOVEBEHINDBALL_H
#define MOVEBEHINDBALL_H
#include "Arduino.h"
#include "SpeedTrig.h"
#include <cmath>
#include "RpiCommunication.h"

// simple one based on proportional angle offset
int decideMotionAngle(int ballAngle);

// more complicated on based the superposition of angle offset + sink + imaginary ball behind
int decideMotionAngle1(int ballAngle, int ballDistance);

int decideMotionAngle2(int ballAngle, int ballDistance);

void mockTesting();

struct Vector
{
	float x, y;
	Vector(float _x, float _y)
	{
		x = _x;
		y = _y;
	}
	// this angle is defined following the convention of analytic geometry
	Vector(int angle, float length)
	{
		x = SpeedTrig.cos(angle) * length;
		y = SpeedTrig.sin(angle) * length;
	}

	void add(Vector v2)
	{
		x = x + v2.x;
		y = y + v2.y;
	}

	Vector mWeight(float w)
	{
		return (Vector(x*w, y*w));
	}

	void normalize()
	{
		float length = sqrt(x*x + y*y);
		x = x / length;
		y = y / length;
	}
};


#endif