#include <opencv2/core/core.hpp>
#include <opencv2/imgcodecs.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <cstring>
#include <opencv2\imgproc.hpp>
#include "Calibration.h"
#include <iostream>
#include <cmath>


using namespace std;
using namespace cv;

bool CALIB = false;

void showImage(string imageName)
{
	Mat inputImage;
	inputImage = imread(imageName, IMREAD_COLOR);
	Size size(1500, 1000);//the dst image size,e.g.100x100
	Mat displayImage;//dst image
	resize(inputImage, displayImage, size);//resize image

	imshow("Win", displayImage);
	if (CALIB) 
		waitKey(0);
}

void showImage(Mat inputImage)
{
	double x = inputImage.size().width;
	double y = inputImage.size().height;
	double scaleFactor = min((1300.0/(double)x ), 800/double(y) );
	//cout << "ScaleFactor" << scaleFactor << "\n";
	Size size(round(x*scaleFactor), round(y*scaleFactor));//the dst image size,e.g.100x100
	Mat displayImage;//dst image
	resize(inputImage, displayImage, size);//resize image

	imshow("Win", displayImage);
	if (CALIB)
		waitKey(0);
}

void selectRed(Mat inputImage)
{
	Mat HSVImage;
	cvtColor(inputImage, HSVImage, COLOR_BGR2HSV);
	Mat thresholdedImage1;
	Mat thresholdedImage2;
	Scalar lowRed1(0, 50, 100);
	Scalar highRed1(20, 255, 255);
	Scalar lowRed2(160, 50, 100);
	Scalar highRed2(179, 255, 255);
	inRange(HSVImage, lowRed1, highRed1, thresholdedImage1); //Threshold the image
	inRange(HSVImage, lowRed2, highRed2, thresholdedImage2); //Threshold the image
	add(thresholdedImage1, thresholdedImage2, thresholdedImage1);
	showImage(thresholdedImage1);
	if (CALIB)
		waitKey(0);
	Mat edges;
	Canny(thresholdedImage1, edges, 255, 255);
	showImage(edges);
	if (CALIB)
		waitKey(0);
	vector<vector<Point>> contours;
	vector<Point>selectedPoints;
	vector<Vec4i> hierarchy;

	findContours(edges, contours, hierarchy, RETR_EXTERNAL, CHAIN_APPROX_NONE, Point(0, 0));

	double max_size = 0;
	vector<int> selected_index;
	for (int i = 0; i < contours.size(); i++)
	{

		// get the bounding rectangle
		Rect R = boundingRect(contours[i]);
		if (R.area() > 800)
		{
			max_size = R.area();
			selected_index.push_back(i);
		}
	}


	Scalar color = Scalar(255, 255, 0);
	for (int i = 0; i < selected_index.size(); i++)
	{
		drawContours(inputImage, contours, (int)selected_index[i], color, 2, 8, hierarchy, 0, Point());
		selectedPoints.insert(selectedPoints.end(), contours[selected_index[i]].begin(), contours[selected_index[i]].end());
		/*for (int j = 0; j<contours[selected_index[i]].size();j++)
			selectedPoints.push_back(contours[selected_index[i]][j]);*/
	}

	polylines(inputImage, selectedPoints, false, color, 1, 8, 0);

	showImage(inputImage);
	if (CALIB)
		waitKey(0);
	//cout << selectedPoints;
	RotatedRect box = fitEllipse(selectedPoints);

	ellipse(inputImage, box, color, 2, 8);
	drawMarker(inputImage, box.center, color, 0, 20, 1, 8);
	showImage(inputImage);
	if (CALIB)
		waitKey(0);
}

void findCentroid(vector<Point> contour, double& x, double &y)
{
	x = 0.0;
	y = 0.0;
	for (int i = 0; i < contour.size();i++)
	{
		x = x + ((double)contour[i].x) / ((double)contour.size());
		y = y + ((double)contour[i].y) / ((double)contour.size());
	}
}

vector<Point> selectBlue(Mat inputImage)
{
	Mat HSVImage;
	cvtColor(inputImage, HSVImage, COLOR_BGR2HSV);
	Mat thresholdedImage;
	// the threshold may need adjustment
	Scalar lowBlue(20, 50, 100);
	Scalar highBlue(30, 255, 255);
	inRange(HSVImage, lowBlue, highBlue, thresholdedImage); //Threshold the image

	//erode and dilate
	int erosion_size = 1;
	Mat element = getStructuringElement(MORPH_RECT,
		Size(2 * erosion_size + 1, 2 * erosion_size + 1),
		Point(erosion_size, erosion_size));
	erode(thresholdedImage, thresholdedImage, element);
	int dilation_size = 3;
	element = getStructuringElement(MORPH_RECT,
		Size(2 * dilation_size + 1, 2 * dilation_size + 1),
		Point(dilation_size, dilation_size));
	dilate(thresholdedImage, thresholdedImage, element);
	showImage(thresholdedImage);
	if (CALIB)
		waitKey(0);

	// contour and centroid
	Mat edges;
	Canny(thresholdedImage, edges, 255, 255);

	vector<vector<Point>> contours;
	vector<Point>selectedPoints;
	vector<Vec4i> hierarchy;
	findContours(edges, contours, hierarchy, RETR_EXTERNAL, CHAIN_APPROX_NONE, Point(0, 0));

	Scalar color = Scalar(255, 0, 0);
	for (int i = 0; i < contours.size(); i++)
		drawContours(inputImage, contours, (int)i, color, 2, 8, hierarchy, 0, Point());
	showImage(inputImage);
	if (CALIB)
		waitKey(0);

	Point points[20];

	for (int i = 0; i < contours.size(); i++)
	{
		double x, y;
		findCentroid(contours[i], x, y);
		points[i].x = x;
		points[i].y = y;
 		drawMarker(inputImage, Point(round(x), round(y)), color, 0, 40, 3, 8);
		cout << Point(round(x), round(y));
	}

	showImage(inputImage);

	// sorting
	for (int i =0; i<contours.size()-1;i++)
		for (int j=i+1;j<contours.size();j++) 
			if (points[i].y > points[j].y)
			{
				Point temp = points[i];
				points[i] = points[j];
				points[j] = temp;
			}

	if (CALIB)
		waitKey(0);

	// convert to the desired output format
	vector<Point> ans;
	for (int i = 0; i < contours.size(); i++)
		ans.push_back(points[i]);
	return ans;
}