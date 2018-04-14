#pragma once
#include "robotPose.h"

robotPose::robotPose()
{
}

robotPose::robotPose(int _x, int _y, int _heading)
{
	init(_x, _y, _heading);
}

void robotPose::init(int _x, int _y, int _heading)
{
	x = _x;
	y = _y;
	heading = _heading;
}