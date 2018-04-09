#include "MotionModel.h"
#include "robotPose.h"

void MotionModel::setMotionInfo(double _motion_v_stdDeviation, double _motion_heading_stdDeviation)
{
	motion_v_stdDeviation = _motion_v_stdDeviation;
	motion_heading_stdDeviation = _motion_heading_stdDeviation;
}

// the heading does not refer to the robot heading but the direction required by motion command
robotPose MotionModel::motionSampler(robotPose previousState, int distance, int heading)
{
	double R1, R2;
	RNGgenerator.gaussian(R1, R2);
	R1 = R1 * motion_v_stdDeviation;
	R2 = R2 * motion_heading_stdDeviation;
	double distance_withNoise = (double)distance * (1 + R1);
	double heading_withNoise = (double)heading * (1 + R2);
	robotPose newPose(round(previousState.x - distance_withNoise*sin(heading_withNoise * 3.1415 / 180.0)), round(previousState.y + distance_withNoise*cos(heading_withNoise * 3.1415 / 180.0)), 0);
	// the weight is only restored to unity when a resampling is done
	return newPose;
}

double MotionModel::calDistance(int x1, int y1, int x2, int y2)
{
	return sqrt((x1 - x2)*(x1 - x2) + (y1 - y2)*(y1 - y2));
}

double MotionModel::motionProb(robotPose previousState, robotPose currentState, int distance, int heading)
{
	double distanceErr = calDistance(previousState.x, previousState.y, currentState.x, currentState.y) / (double)distance - 1.0;
	// the heading is not yet taken into account
	return distributionCalculator.gaussian(distanceErr, motion_v_stdDeviation);
}