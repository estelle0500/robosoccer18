#pragma once
#include <opencv2/core/core.hpp>
#include <opencv2/imgcodecs.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <cstring>
#include <opencv2\imgproc.hpp>
#include "Calibration.h"
#include <iostream>
#include "PerspectiveCorrection.h"

Mat findTransformationMatrix(vector<Point> distorted, vector<Point> undistorted)
{
	// convert the data format so that it can be used by the algorithm
	Point2f distorted2[4];
	Point2f undistorted2[4];
	cout << "conversion\n";
	for (int i = 0; i < 4; i++)
	{
		distorted2[i] = Point2f((float)distorted[i].x, (float)distorted[i].y);
		undistorted2[i] = Point2f((float)undistorted[i].x, (float)undistorted[i].y);
		cout << distorted2[i] << " " << undistorted2[i] << "\n";
	}

	Mat ans = getPerspectiveTransform(distorted2, undistorted2);
}

Mat imageToReal(vector<Point> distorted, vector<Point> undistorted)
{
	return findTransformationMatrix(distorted, undistorted);
}

Mat realToImage(vector<Point> distorted, vector<Point> undistorted)
{
	return findTransformationMatrix(undistorted, distorted);
}


Point mapPoint(Mat tMatrix, Point originalPos)
{
	double x1 = tMatrix.at < double >(0, 0) * originalPos.x + tMatrix.at<double>(0, 1) * originalPos.y + tMatrix.at<double>(0, 2);
	double x2 = tMatrix.at<double>(1, 0) * originalPos.x + tMatrix.at<double>(1, 1) * originalPos.y + tMatrix.at<double>(1, 2);
	double x3 = tMatrix.at<double>(2, 0)* originalPos.x + tMatrix.at<double>(2, 1) * originalPos.y + tMatrix.at<double>(2, 2);
	return Point(round(x1 / x3), round(x2 / x3));
}
