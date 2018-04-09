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
	double sHit = pHit * distributionCalculator.gaussian(expectedReading - sensorReading, sensorStd);
	double sRand = pRand * 1.0 / (double)maxRange;
	double sShort = pShort * distributionCalculator.expDist(sensorReading, expectedReading, lamdaShort);
	return sHit + sRand + sShort;
}

double SensorModel::mergeSensorProb(sensorReading rawReading, robotPose pose)
{
	double probability = 1;
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


void SensorModel::probFieldInit(int grid_x, int grid_y)
{
	xSize = grid_x;
	ySize = grid_y;
	// initialize the field using dynamic array
	probField = vector<vector<double>>(xSize, vector<double>(ySize, 0));
	xWidth = (double)fieldWidth / (double)xSize;
	yWidth = (double)fieldLength / (double)ySize;
}

void SensorModel::calProbabilityField(sensorReading &rawReading)
{
	sensorReadingFilter(rawReading);
	for (int i = 0; i < xSize; i++)
		for (int j = 0; j < ySize; j++)
		{
			int x = (int)(i*xWidth + xWidth / 2.0);
			int y = (int)(j*yWidth + yWidth / 2.0);
			robotPose pose(x, y, 0);
			probField[i][j] = mergeSensorProb(rawReading, pose);
		}
}

/*
Test code segment
void printCompatible2(double number)
{
	ffout << "Internal`StringToDouble[\"" << number << "\"]";
}

int main()
{
	SensorModel sensorModel;
	ffout << "ListPlot[{";
	for (int i = 0; i < 183; i++)
	{
		robotPose pose(50, 150, 0);
		double prob = sensorModel.sensorReadingProb(0, i, pose);
		ffout << "{" << i << ",";
		printCompatible2(prob);
		ffout << "},";
	}
	ffout << "{0,0}}]";
	ffout.close();
	return 0;
}
*/
