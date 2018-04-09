#pragma once
#include "robotPose.h"
#include "Stats.h"
#include "fastRNG.h"

class MotionModel
{
public:
	//setter
	void setMotionInfo(double _motion_v_stdDeviation, double _motion_heading_stdDeviation);

	/**
	Generate the next possible robot position based on motion command data.
	The percentage error in the speed and heading is a gaussian with the standard deviation stored in the class.

	@param previousState The previous state before the motion command.
	@param v Not quite the speed but the distance the robot is expected to move
	@param heading Robot heading. Measured anticlockwise from "north"
	@return The sampled robot position after the motion command
	*/
	robotPose motionSampler(robotPose previousState, int distance, int heading);
	double motionProb(robotPose previousState, robotPose currentState, int distance, int heading);

private:
	double motion_v_stdDeviation = 0.1;
	double motion_heading_stdDeviation = 0.1;
	Stats distributionCalculator;
	fastRNG RNGgenerator;
	double calDistance(int x1, int y1, int x2, int y2);
};