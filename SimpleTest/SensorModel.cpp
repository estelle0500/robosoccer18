#include "SensorModel.h"
#include "robotPose.h"

// not tested or tuned

void SensorModel::setSensorInfo(int _sensorStd, float _pHit, float _pRand, float _pShort, float _lamdaShort)
{
	sensorStd = _sensorStd;
	pHit = _pHit;
	pRand = _pRand;
	pShort = _pShort;
	lamdaShort = _lamdaShort;
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
	float sRand = pRand * 1.0 / (float)maxRange;
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


void SensorModel::probFieldInit()
{
	xSize = GRID_X;
	ySize = GRID_X;
	// initialize the field using dynamic array
	xWidth = (float)fieldWidth / (float)xSize;
	yWidth = (float)fieldLength / (float)ySize;
}

void SensorModel::calProbabilityField(sensorReading &rawReading)
{
	float maxProb = -1.0;
	sensorReadingFilter(rawReading);
	for (int i = 0; i < xSize; i++)
		for (int j = 0; j < ySize; j++)
		{
			int x = (int)(i*xWidth + xWidth / 2.0);
			int y = (int)(j*yWidth + yWidth / 2.0);
			robotPose pose(x, y, 0);
			probField[i][j] = mergeSensorProb(rawReading, pose);
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
