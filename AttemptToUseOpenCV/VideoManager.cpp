#pragma once
#include <opencv2/core/core.hpp>
#include <opencv2/imgcodecs.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <cstring>
#include <opencv2\imgproc.hpp>
#include "Calibration.h"
#include <iostream>
#include "VideoManager.h"
#include "patternIdentification.h"

using namespace std;
using namespace cv;

void readVideo(string fileName)
{
	VideoCapture video = VideoCapture(fileName);
	Mat currentFrame;
	for (int i =0; i<100;i++)
	{
		video.read(currentFrame);
		showImage(currentFrame);
		waitKey(0);
	}
}

void PatternIdentificationVideo(string fileName)
{
	VideoCapture video = VideoCapture(fileName);
	Mat currentFrame;
	for (int i = 0;i<500;i++)
		video.read(currentFrame);

	initROI(currentFrame);
	for (int i = 0; i<1000; i++)
	{
		extractPatternPoints(currentFrame);
		for (int j = 0; j<5;j++)
			if (!video.read(currentFrame))
				return;
		//showImage(currentFrame);
		if (waitKey(1) & 0xFF == 'q')
			break;
		//waitKey(1);
	}
}