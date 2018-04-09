#include "SensorModel.h"
#include "robotPose.h"

// not tested or tuned

void SensorModel::setSensorInfo(int _sensorStd, double _pHit, double _pRand, double _pShort, double _lamdaShort)
{
	sensorStd = _sensorStd;
	pHit = _pHit;
	pRand = _pRand;
	pShort = _pShort;
	lamdaShort = _lamdaShort;
}

double SensorModel::sensorReadingProb(int sensorInd, int sensorReading, robotPose pose)
{
	// first compute the ray tracing result
	// the speical problems with the front sensor will be later handled
	int expectedReading;
	if (sensorInd == 0) // front
	{
		if (pose.x < (fieldWidth - goalWidth) / 2 || pose.x >(fieldWidth + goalWidth) / 2)
			expectedReading = fieldLength + frontBackMargin - pose.y;
		else
			expectedReading = fieldLength - goalDepth - pose.y;
	}
	if (sensorInd == 1) //left
		expectedReading = pose.x + sideMargin;
	if (sensorInd == 2) //back
	{
		if (pose.x < (fieldWidth - goalWidth) / 2 || pose.x >(fieldWidth + goalWidth) / 2)
			expectedReading = frontBackMargin + pose.y;
		else
			expectedReading = -goalDepth + pose.y;
	}
	if (sensorInd == 3) //right
		expectedReading = -pose.x + sideMargin + fieldWidth;
	expectedReading = expectedReading - sensorDisplacement[sensorInd];
	// then proceed to calculate the probability
	// a random factor in front to prevent an excessively small number
	return (1E+1)*(pHit * distributionCalculator.gaussian(expectedReading - sensorReading, sensorStd) + pRand * 1.0 / (double)expectedReading + pShort * distributionCalculator.expDist(sensorReading, expectedReading, lamdaShort));
}