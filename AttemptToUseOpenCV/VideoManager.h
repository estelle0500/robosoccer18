#pragma once
#include <opencv2/core/core.hpp>
#include <opencv2/imgcodecs.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <cstring>
#include <opencv2\imgproc.hpp>
#include "Calibration.h"
#include <iostream>

using namespace std;
using namespace cv;

void readVideo(string fileName);

void PatternIdentificationVideo(string fileName);