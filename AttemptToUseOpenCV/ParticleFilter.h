#pragma once
#include <cmath>
#include <vector>
#include "robotPose.h"
#include "fastRNG.h"
#include "Stats.h"
#include "SensorModel.h"
#include "MotionModel.h"

#ifndef COMPILE_FOR_ARDUINO
	#include <iostream>
#endif

//#define VISUALIZATION_ENABLED
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
	void update(int v, int heading, SensorModel::sensorReading rawReading);

	/*
	Obtain the mostlikely position based on currentParticles
	@param method The method used 0 : centroid, 1 : max density box (not implemented yet)
	*/
	robotPose obtainPosition(int method);

	// setters for basic position
	void setSensorInfo(int _sensorStd, double _pHit, double _pRand, double _pShort, double lamdaShort);
	void setMotionInfo(double _motion_v_stdDeviation, double _motion_heading_stdDeviation);

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
	SensorModel sensorModel;
	MotionModel motionModel;
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
	
	//Motion update segment

	/*
	Constrain the particles to within the bound
	@param raw The raw robot pose
	*/
	void constrain(element &raw);

	/*
	Based on motion command, generate projectedParticles from currentParticles
	@param distance The distance the robot is expected to move
	@param heading Robot heading. Measured anticlockwise from "north"
	*/
	void motionUpdate(int distance, int heading);

	//Sensor update segment
	/*
	Generate currentParticles from the projectedParticles based on motion update.
	Resampling is done using low variance sampler at every step as of now
	@param rawReading The received sensor reading.
	*/
	void sensorUpdate(SensorModel::sensorReading rawReading);

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