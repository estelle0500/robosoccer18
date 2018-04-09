#pragma once

#include <opencv2/core/core.hpp>
#include <opencv2/imgcodecs.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <cstring>

using namespace std;
using namespace cv;

void showImage(string imageName);

void showImage(Mat inputImage);

void selectRed(Mat inputImage);

vector<Point> selectBlue(Mat inputImage);