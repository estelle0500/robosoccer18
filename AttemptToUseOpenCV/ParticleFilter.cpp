#pragma once

#include "ParticleFilter.h"
#include "fastRNG.h"
#include <cmath>
#include "Stats.h"
#include <iostream>

using namespace std;

#ifdef VISUALIZATION_ENABLED
#include <opencv2/core/core.hpp>
#include <opencv2/imgcodecs.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <cstring>
#include <opencv2/imgproc.hpp>
using namespace cv;
#endif

// working
ParticleFilter::ParticleFilter(int _particleNumber)
{
	particleNumebr = _particleNumber;
	currentParticles.clear();
	// start with a uniform distribution
	for (int i = 0; i < particleNumebr; i++)
	{
		// it is assumed that the robot will approximately face the front, so the RNGed initial poses are all facing the front
		// need to check the RNG func
		robotPose pose((generator.RNG())*fieldWidth, (generator.RNG())*fieldLength, 0);
		currentParticles.push_back(element(pose, 1));
	}
}

void ParticleFilter::setSensorInfo( int _sensorStd, double _pHit, double _pRand, double _pShort, double _lamdaShort)
{
	sensorModel.setSensorInfo(_sensorStd, _pHit, _pRand, _pShort, _lamdaShort);
}

void ParticleFilter::setMotionInfo(double _motion_v_stdDeviation, double _motion_heading_stdDeviation)
{
	motionModel.setMotionInfo(_motion_v_stdDeviation, _motion_heading_stdDeviation);
}

void ParticleFilter::constrain(ParticleFilter::element &raw)
{
	if (raw.pose.x < 0)
		raw.pose.x = 0;
	if (raw.pose.x > fieldWidth)
		raw.pose.x = fieldWidth;
	if (raw.pose.y < 0)
		raw.pose.y = 0;
	if (raw.pose.y > fieldLength)
		raw.pose.y = fieldLength;
	return;
}

void ParticleFilter::motionUpdate(int distance, int heading)
{
	projectedParticles.clear();
	for (int i = 0; i < currentParticles.size(); i++)
	{
		robotPose newPose = motionModel.motionSampler(currentParticles[i].pose, distance, heading);
		projectedParticles.push_back(element(newPose,1));
		constrain(projectedParticles[i]);
	}
}


void ParticleFilter::lowVarianceSampler(double startingPt, double step)
{
	currentParticles.clear();
	double residual = startingPt;
	for (int i = 0; i < projectedParticles.size(); i++)
	{
		while (projectedParticles[i].weight > residual)
		{
			currentParticles.push_back(projectedParticles[i].copy());
			currentParticles[currentParticles.size()-1].weight = 1;
			projectedParticles[i].weight -= residual;
			residual = step;
		}
		residual = residual - projectedParticles[i].weight;
	}
}

// everything before sampling seems to be ok
void ParticleFilter::sensorUpdate(SensorModel::sensorReading rawReading)
{
	// remove outrageous readings
	sensorModel.sensorReadingFilter(rawReading);
	
	//scan through the entire list of projected particles to calculate the weight
	for (int i = 0; i < projectedParticles.size(); i++)
		projectedParticles[i].weight = sensorModel.mergeSensorProb(rawReading, projectedParticles[i].pose);
	//printOutAll(1);

	// sampling decision is to be made here
	// as of now the resampling will take place after every round
	double totalWeight = 0;
	for (int i = 0; i < projectedParticles.size(); i++)
	{
		//cout <<i<<" "<< totalWeight<<" "<< projectedParticles[i].weight<<"\n";
		totalWeight = totalWeight + projectedParticles[i].weight;
	}
	lowVarianceSampler(generator.RNG() * totalWeight / particleNumebr, totalWeight/particleNumebr);
}


void ParticleFilter::update(int v, int heading, SensorModel::sensorReading rawReading)
{
	motionUpdate(v, heading);
	sensorUpdate(rawReading);
	// make sure the the number is restored back to the expected number of particles.
	makeUpToNumber();
}

robotPose ParticleFilter::obtainPosition(int method)
{
	// the centroid method
	if (method == 0)
	{
		int x = 0;
		int y = 0;
		for (int i = 0; i < currentParticles.size(); i++)
		{
			x = x + currentParticles[i].pose.x;
			y = y + currentParticles[i].pose.y;
		}
		return robotPose(x / currentParticles.size(), y / currentParticles.size(), 0);
	}
	// the searching box method, grid method
	return robotPose(0, 0, 0);
}

#ifndef COMPILE_FOR_ARDUINO
void ParticleFilter::printOutAll(int cmd)
{
	cout << "ListPointPlot3D[{";
	if (cmd == 0)
		for (int i = 0; i < currentParticles.size(); i+=1)
			cout << "{" << currentParticles[i].pose.x << "," << currentParticles[i].pose.y << ",Internal`StringToDouble[\"" << currentParticles[i].weight<< "\"]},";
	else
		for (int i = 0; i < projectedParticles.size(); i+=1)
			cout << "{" << projectedParticles[i].pose.x << "," << projectedParticles[i].pose.y << ",Internal`StringToDouble[\"" << projectedParticles[i].weight << "\"]},";
	cout << "{0,0,0},{122,183,0}}]";
	char c;
	cin >> c;
	return;
}
#endif

#ifdef COMPILE_FOR_ARDUINO
void ParticleFilter::printOutAll(int cmd)
{}
#endif

void ParticleFilter::makeUpToNumber()
{
	while (currentParticles.size() < particleNumebr)
	{
		robotPose pose((generator.RNG() + 0.5)*fieldWidth, (generator.RNG() + 0.5)*fieldLength, 0);
		// it is assumed that this is done after a resampling process, so the weight is placed as one instead.
		currentParticles.push_back(element(pose, 1));
	}
}


#ifdef VISUALIZATION_ENABLED
// the image to contain the display
void ParticleFilter::resetDisplay()
{
	displayImg =  Mat(183*5, 122*5, CV_8UC3, Scalar(255, 255, 255));
}
void ParticleFilter::draw(vector<element> pointList, char colorCode)
{
	Scalar color = Scalar(0, 0, 0);
	if (colorCode == 'B')
		color = Scalar(255, 0, 0);
	if (colorCode == 'R')
		color = Scalar(0, 0, 255);
	if (colorCode == 'G')
		color = Scalar(0, 255, 0);
	for (int i = 0; i < pointList.size(); i++)
	{
		circle(displayImg, Point(pointList[i].pose.x*5, pointList[i].pose.y*5), 2, color, 2, 8, 0);
	}
}

void ParticleFilter::showImage(String message)
{
	showImage(message, displayImg);
}

void ParticleFilter::showImage(String message, Mat inputImage)
{
	cout << message << "\n";
	double x = inputImage.size().width;
	double y = inputImage.size().height;
	double scaleFactor = min((1300.0 / (double)x), 800 / double(y));
	//cout << "ScaleFactor" << scaleFactor << "\n";
	Size size(round(x*scaleFactor), round(y*scaleFactor));//the dst image size,e.g.100x100
	Mat ddisplayImage;//dst image
	resize(inputImage, ddisplayImage, size);//resize image

	imshow("Win", ddisplayImage);
	waitKey(0);
}
#endif
