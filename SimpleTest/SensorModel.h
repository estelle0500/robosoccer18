#ifndef SENSORMODEL_H
#define SENSORMODEL_H


#include "robotPose.h"
#include "Stats.h"

#ifdef COMPILE_FOR_ARDUINO
#include <math.h>
#endif // COMPILE_FOR_ARDUINO
#ifndef COMPILE_FOR_ARDUINO
#include <cmath>
#endif // COMPILE_FOR_ARDUINO

#define GRID_X 15
#define GRID_Y 15

using namespace std;

class SensorModel
{
public:
	// struct to contain a set of sensor readings
	struct sensorReading
	{
		int distance[4];
		sensorReading(int front, int left, int back, int right)
		{
			distance[0] = front;
			distance[1] = left;
			distance[2] = back;
			distance[3] = right;
		}
	};

	// setter
	void setSensorInfo(int _sensorStd, float _pHit, float _pRand, float _pShort, float lamdaShort);

	/**
	Find the possibility of the sensor reading a value using ray tacing.
	@param sensorInd Indicate the sensor of concern here. The coding follows that in the struct
	@param sensorReading The raw value returned by the sensor.
	@param pose The expected position where the reading is taken.
	@return the likelihood of the obtained reading given that the robot is at the pose
	*/
	float sensorReadingProb(int sensorInd, int sensorReading, robotPose pose);
	float mergeSensorProb(sensorReading rawReading, robotPose pose);

	/**
	Remove sensor readings that are unphysical and likely short readings (not yet implemented).
	The algorithm will use the projected particles from motion update to reject short readings.
	All the rejected readings will be turned to -1 for marking.
	Accepted readings will be offset by sensor displacement
	@param rawReading The received sensor reading.
	*/
	void sensorReadingFilter(sensorReading &rawReading);

	// for the histogram filter
	void probFieldInit();
	int xSize;
	int ySize;
	float xWidth;
	float yWidth;
	float probField[GRID_X][GRID_Y];

	void calProbabilityField(sensorReading &rawReading);
	robotPose mostLikelyPose;
	// obtain the most likely robotPose based on data calculated using probability field
	robotPose getMostLikely();

private:
	// some information about the field
	int fieldLength = 183;
	int fieldWidth = 122;
	int sideMargin = 30;
	int frontBackMargin = 30;
	int goalWidth = 60;
	int goalDepth = 0;

	//some information about the robot
	int sensorDisplacement[4] = { 10,10,10,10 };
	int sensorStd = 10;
	int maxRange = 200;
	float pHit = 0.5, pRand = 0.2, pShort = 0.3; // these value needs to be tuned
	float lamdaShort = 1.0 / 30.0; // inversly related to the distance
	Stats distributionCalculator;

};


#endif // SENSORMODEL_H


