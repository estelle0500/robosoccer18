#include "SensorModel.h"
#include "robotPose.h"
#include <iostream>
// this version is somewhat working / correct
// not tested or tuned

void SensorModel::setSensorInfo(int _sensorStd, float _pHit, float _pRand, float _pShort, float _lamdaShort)
{
	sensorStd = _sensorStd;
	pHit = _pHit;
	pRand = _pRand;
	pShort = _pShort;
	lamdaShort = _lamdaShort;
}


void SensorModel::probFieldInit()
{
	xSize = GRID_X;
	ySize = GRID_X;
	// initialize the field using dynamic array
	xWidth = (float)fieldWidth / (float)xSize;
	yWidth = (float)fieldLength / (float)ySize;
	reset();
}

float SensorModel::sensorReadingProb(int sensorInd, int sensorReading, robotPose pose)
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
	float sHit = pHit * distributionCalculator.gaussian(expectedReading - sensorReading, sensorStd);
	float sRand = pRand * 1.0f / (float)maxRange;
	float sShort = pShort * distributionCalculator.expDist(sensorReading, expectedReading, lamdaShort);
	return sHit + sRand + sShort;
}

float SensorModel::mergeSensorProb(sensorReading rawReading, robotPose pose)
{
	float probability = 1;
	for (int j = 0; j < 4; j++)
		if (rawReading.distance[j] != -1)
			probability = probability * sensorReadingProb(j, rawReading.distance[j], pose);
	if (isnan(probability))
		probability = 0;
	return probability;
}


void SensorModel::sensorReadingFilter(SensorModel::sensorReading &rawReading)
{
	//front
	if (rawReading.distance[0] > fieldLength + frontBackMargin) rawReading.distance[0] = -1;
	//back
	if (rawReading.distance[2] > fieldLength + frontBackMargin) rawReading.distance[2] = -1;
	//left
	if (rawReading.distance[1] > fieldWidth + sideMargin) rawReading.distance[1] = -1;
	//right
	if (rawReading.distance[3] > fieldWidth + sideMargin) rawReading.distance[3] = -1;
}

void SensorModel::calProbabilityField(sensorReading &rawReading)
{

	float maxProb = -1.0;
	sensorReadingFilter(rawReading);

	// the rejection and reset code should be added here

	for (int i = 0; i < xSize; i++)
		for (int j = 0; j < ySize; j++)
		{
			int x = (int)(i*xWidth + xWidth / 2.0);
			int y = (int)(j*yWidth + yWidth / 2.0);
			robotPose pose(x, y, 0);
			probField[i][j] = probField_prior[i][j] * mergeSensorProb(rawReading, pose);
			if (probField[i][j] > maxProb)
			{
				maxProb = probField[i][j];
				mostLikelyPose = pose;
			}
		}
}

robotPose SensorModel::getMostLikely()
{
	float maxProb = -1.0;
	int xMax = -1;
	int yMax = -1;
	for (int i = 0; i < xSize; i++)
		for (int j = 0; j < ySize; j++)
		{
			if (probField[i][j] > maxProb)
			{
				maxProb = probField[i][j];
				xMax = i;
				yMax = j;
			}
		}
	return robotPose(xMax, yMax, 0);
}

float SensorModel::dist(float x1, float y1, float x2, float y2)
{
	return sqrt((x1 - x2)*(x1 - x2) + (y1 - y2)*(y1 - y2));
}


// motion update segment
// the angle is the angle measured from the north
// not so perfectly correct, but should be kind of ok
void SensorModel::createUpdateList(int angle)
{
	updateListLength = 0;
	int deltaX = (int)round(driftDistance*SpeedTrig.cos(angle + 90));
	int deltaY = (int)round(driftDistance*SpeedTrig.sin(angle + 90));
	// rounding up
	int lowerX = (deltaX - (int)circleRad - 1) / xWidth;
	int upperX = (deltaX + (int)circleRad + 1) / xWidth;
	int lowerY = (deltaY - (int)circleRad - 1) / yWidth;
	int upperY = (deltaY + (int)circleRad + 1) / yWidth;

	for (int i = lowerX; i <= upperX; i++)
		for (int j = lowerY; j <= upperY; j++)
			// all with reference to the original point
			if (dist(i*xWidth, j*yWidth, deltaX, deltaY) < circleRad)
			{
				updateList[0][updateListLength] = i;
				updateList[1][updateListLength] = j;
				updateListLength++;
				//cout << "{" << i << "," << j << "},";
			}
}

void SensorModel::motionUpdate()
{
	for (int i = 0; i < xSize; i++)
		for (int j = 0; j < ySize; j++)
			probField_prior[i][j] = 0;
	for (int i = 0; i < xSize; i++)
		for (int j = 0; j < ySize; j++)
		{
			for (int k = 0; k < updateListLength; k++)
			{
				int uI = updateList[0][k] + i;
				int uJ = updateList[1][k] + j;
				if (uI >= 0 && uI < xSize && uJ >= 0 && uJ < ySize)
					probField_prior[uI][uJ] += probField[i][j];
			}
			//getchar();
		}
	normalize();
	// to keep a certain level of minProb to prevent localization failure
	for (int i = 0; i < xSize; i++)
		for (int j = 0; j < ySize; j++)
			if (probField_prior[i][j] < minProb)
				probField_prior[i][j] = minProb;
}

void SensorModel::reset()
{
	for (int i = 0; i < xSize; i++)
		for (int j = 0; j < ySize; j++)
		{
			probField_prior[i][j] = 1;
			probField[i][j] = 1;
		}
}

// normalize the prior field
void SensorModel::normalize()
{
	float totalProb = 0.0f;
	for (int i = 0; i < xSize; i++)
		for (int j = 0; j < ySize; j++)
			totalProb += probField_prior[i][j];
	//cout << totalProb << "\r\n";
	for (int i = 0; i < xSize; i++)
		for (int j = 0; j < ySize; j++)
			probField_prior[i][j] = probField_prior[i][j] / totalProb;
}










/*
Test code segment
void printCompatible2(float number)
{
ffout << "Internal`StringTofloat[\"" << number << "\"]";
}

int main()
{
SensorModel sensorModel;
ffout << "ListPlot[{";
for (int i = 0; i < 183; i++)
{
robotPose pose(50, 150, 0);
float prob = sensorModel.sensorReadingProb(0, i, pose);
ffout << "{" << i << ",";
printCompatible2(prob);
ffout << "},";
}
ffout << "{0,0}}]";
ffout.close();
return 0;
}
*/
