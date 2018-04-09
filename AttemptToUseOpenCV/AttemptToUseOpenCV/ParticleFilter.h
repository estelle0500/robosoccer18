#pragma once
#include "robotPose.h"
#include <vector>
#include "fastRNG.h"
#include <cmath>
#include "Stats.h"
#include <iostream>

#define VISUALIZATION_ENABLED
#ifdef VISUALIZATION_ENABLED
	#include <opencv2/core/core.hpp>
	#include <opencv2/imgcodecs.hpp>
	#include <opencv2/highgui/highgui.hpp>
	#include <opencv2/imgproc.hpp>
	#include <cstring>
	using namespace cv;
#endif

using namespace std;

class ParticleFilter {
public:
	struct element
	{
		robotPose pose;
		double weight;
		element(robotPose _pose, double _weight)
		{
			pose.init(_pose.x, _pose.y, _pose.heading);
			weight = _weight;
		}
		element copy()
		{
			return element(pose,weight);
		}

	};

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

	/*
	Constructor. Initialize the filter by generating particles uniformly distributed on the field
	@param _particleNumber The number of particles to be used in the programme.
	*/
	ParticleFilter(int _particleNumber);
	
	/*
	The main function to call to update sensor reading and motion command
	@param v Not quite the speed but the distance the robot is expected to move
	@param heading Robot heading. Measured anticlockwise from "north"
	@param rawReading The received sensor reading.
	*/
	void update(int v, int heading, sensorReading rawReading);

	/*
	Obtain the mostlikely position based on currentParticles
	@param method The method used 0 : centroid, 1 : max density box (not implemented yet)
	*/
	robotPose obtainPosition(int method);

	// setters for basic position
	void setSensorInfo(int _sensorStd, double _pHit, double _pRand, double _pShort, double lamdaShort);
	void setMotionInfo(double _motion_v_variance, double _motion_heading_variance);

	// debug functions to print out all currentParticles
	void printOutAll(int cmd);

	// functions for visualization
#ifdef VISUALIZATION_ENABLED
	// the image to contain the display
	Mat displayImg = Mat(122*5, 183*5, CV_8UC3, Scalar(0, 0, 0));
	void resetDisplay();
	void draw(vector<element> pointList, char color);
	// the image will be automatically resized
	void showImage(String message);
	void showImage(String message, Mat inputImage);
#endif

	vector<element> currentParticles;
	vector<element> projectedParticles;
private:
	
	fastRNG generator;
	int cycle_counter;
	int particleNumebr;

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
	double pHit = 1, pRand = 0, pShort = 0; // these value needs to be tuned
	double lamdaShort = 1.0;

	// information about the robot
	double motion_v_variance=0.1;
	double motion_heading_variance = 0.1;

	//Motion update segment

	/**
	Generate the next possible robot position based on motion command data.
	The percentage error in the speed and heading is a gaussian with the variance stored in the class.

	@param previousState The previous state before the motion command.
	@param v Not quite the speed but the distance the robot is expected to move
	@param heading Robot heading. Measured anticlockwise from "north"
	@return The sampled robot position after the motion command
	*/
	element motionSampler(element previousState, int v, int heading);

	/*
	Based on motion command, generate projectedParticles from currentParticles
	@param v Not quite the speed but the distance the robot is expected to move
	@param heading Robot heading. Measured anticlockwise from "north"
	*/
	void motionUpdate(int v, int heading);

	//Sensor update segment

	/**
	Remove sensor readings that are unphysical and likely short readings (not yet implemented).
	The algorithm will use the projected particles from motion update to reject short readings.
	All the rejected readings will be turned to -1 for marking.
	Accepted readings will be offset by sensor displacement
	@param rawReading The received sensor reading.
	*/
	void sensorReadingFilter(sensorReading &rawReading);

	/**
	Find the possibility of the sensor reading a value using ray tacing.
	@param sensorInd Indicate the sensor of concern here. The coding follows that in the struct
	@param sensorReading The raw value returned by the sensor.
	@param pose The expected position where the reading is taken.
	@return the likelihood of the obtained reading given that the robot is at the pose
	*/
	double sensorReadingProb(int sensorInd, int sensorReading, element pose);

	/*
	Generate currentParticles from the projectedParticles based on motion update.
	Resampling is done using low variance sampler at every step as of now
	@param rawReading The received sensor reading.
	*/
	void sensorUpdate(sensorReading rawReading);

	// other untilities
	Stats distributionCalculator;

	/*
	Constrain the particles to within the bound
	@param raw The raw robot pose
	*/
	void constrain(element &raw);

	/*
	Implement low variance sampling 
	@param startingPt A number < step where the sampling starts
	@param step Should be totalWeight/particleNumber
	*/
	void lowVarianceSampler(double startingPt, double step);

	/*
	Because of the loss of resolution in the resampling process or deliberate effort to introduce random poses, the
	size of currenrParticles might be less than the desired particleNumber.
	This function makes up the size, but it can only be called after a resampling process
	*/
	void makeUpToNumber();

	
};