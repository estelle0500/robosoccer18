#pragma once
#include <opencv2/core/core.hpp>
#include <opencv2/imgcodecs.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <cstring>
#include <opencv2\imgproc.hpp>
#include "Calibration.h"
#include <iostream>

Mat findTransformationMatrix(vector<Point> distorted, vector<Point> undistorted);

Point mapPoint(Mat tMatrix, Point originalPos);