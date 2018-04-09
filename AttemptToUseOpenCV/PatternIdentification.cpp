#include <opencv2/core/core.hpp>
#include <opencv2/imgcodecs.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <cstring>
#include <opencv2\imgproc.hpp>
#include "Calibration.h"
#include <iostream>
#include "patternIdentification.h"


using namespace std;
using namespace cv;

int thresholdToDiscard = 15;
int ROIexpand = 30;
bool have_Init = false;

int yCenter = 0; // to store the previous value of center to update ROI
Rect2d maxROI; // to store the largest region the ROI can get to
Rect2d r; // the current ROI

void scaleRect(Rect2d& rect, double scaleFactor)
{
	rect.x = round((double)rect.x / scaleFactor);
	rect.y = round((double)rect.y / scaleFactor);
	rect.width = round((double)rect.width/ scaleFactor);
	rect.height = round((double)rect.height/ scaleFactor);
}

void initROI(Mat inputImage)
{
	have_Init = true;

	double x = inputImage.size().width;
	double y = inputImage.size().height;
	double scaleFactor = min((1000.0 / (double)x), 800 / double(y));
	Size size(round(x*scaleFactor), round(y*scaleFactor));//the dst image size,e.g.100x100
	Mat displayImage;//dst image
	resize(inputImage, displayImage, size);//resize image

	maxROI = selectROI("select maxROI", displayImage);
	destroyWindow("select maxROI");
	scaleRect(maxROI, scaleFactor);
	cout << "maxROI" << maxROI.x << " " << maxROI.y << "\n";

	r = selectROI("ROI win", displayImage);
	destroyWindow("ROI win");
	r = r & maxROI;
	scaleRect(r, scaleFactor);
	cout << "rec " << r.x << " " << r.y << "\n";
	return;
}


// if isInit, then use the selected ROI r
// otherwise, update ROI r based on previous y value
void extractPatternPoints(Mat inputImage)
{
	if (!have_Init)
	{
		cout << "ERROR, not initialized!\n";
		getchar();
		return;
	}
	// cut out ROI
	
	Mat ROI = inputImage(r);
	//showImage(ROI);
	//waitKey(0);

	Mat HSVImage,GreyScaleImage,thresholded;
	cvtColor(ROI, HSVImage, COLOR_BGR2HSV);
	//cvtColor(ROI, GreyScaleImage, COLOR_BGR2GRAY);
	//Mat thresholdedImage;

	// the threshold may need adjustment
	//Scalar lowGreen(50, 50, 50);
	//Scalar highGreen(70, 255, 255);
	//inRange(HSVImage, lowGreen, highGreen, thresholdedImage); //Threshold the image
	//showImage(thresholdedImage);
	//waitKey(0);
	Mat edges;
	// Need to check threshold 50, 150
	vector<Mat> channels;
	split(HSVImage, channels);
	Canny(channels[2], edges, 40, 120);
	//showImage(edges);
	//waitKey(0);
	Mat BGREdges;
	cvtColor(edges, BGREdges, COLOR_GRAY2BGR);
	//BGREdges.copyTo(inputImage(cv::Rect(r.x, r.y, edges.cols, edges.rows)));
	addWeighted(BGREdges, 1, ROI, 1, 0, ROI);
	//showImage(inputImage);
	//waitKey(0);

	vector<Point> pointList = bin2Points(edges, Point(r.x, r.y));
	rectangle(inputImage, r, Scalar(0, 0, 255), 1, 8, 0);
	for (int i = 0; i < pointList.size(); i+=10)
		drawMarker(inputImage, pointList[i], Scalar(0, 0, 255), 0, 10, 1, 8);
	showImage(inputImage);
	//waitKey(0);

}


// this function will also help to update the ROI for the next frame based on the current frame
vector<Point> bin2Points(Mat binaryInput,Point offset)
{
	vector<vector<Point>> contours;
	vector<Vec4i> hierarchy;
	vector<Point> ans;

	findContours(binaryInput, contours, hierarchy, RETR_EXTERNAL, CHAIN_APPROX_NONE, Point(0, 0));
	yCenter = 0; 
	int number_of_points = 0;
	for (int i = 0; i < contours.size(); i++)
	{
		for (int j = 0; j < contours[i].size(); j++)
		{
			number_of_points++;
			yCenter = yCenter + contours[i][j].y;
		}		
	}

	if (number_of_points == 0)
		return ans;

	yCenter = yCenter / number_of_points;
	cout << "yCenter:" << yCenter << "\n";
	int y[10000];
	for (int i = 0; i < 10000; i++)
		y[i] = -1;

	for (int i = 0; i < contours.size(); i++)
		for (int j = 0; j < contours[i].size(); j++)
		{
			if (y[contours[i][j].x] == -1) // not occupied
				y[contours[i][j].x] = contours[i][j].y;
			else
				if (abs(y[contours[i][j].x] - contours[i][j].y) <= thresholdToDiscard)
					y[contours[i][j].x] = (y[contours[i][j].x] + contours[i][j].y) / 2;
				else if (abs(y[contours[i][j].x] - yCenter) > abs(contours[i][j].y - yCenter)) // update if it is close
					y[contours[i][j].x] = contours[i][j].y;
		}

	int yMax = 0;
	int yMin = 10000;
	for (int x = 0; x < 10000; x++)
		if (y[x] != -1)
		{
			ans.push_back(Point(x + offset.x, y[x] + offset.y));
			if (y[x] > yMax)
				yMax = y[x];
			if (y[x] < yMin)
				yMin = y[x];
		}
	

	// ROI update
	int r_y = yMin - ROIexpand+ offset.y; // expand the size slightly
	int r_height = yMax - yMin + 2 * ROIexpand; // not sure if necessary to set the min size
	r.y = r_y;
	r.height = r_height;
	r = r & maxROI;
	return ans;

}