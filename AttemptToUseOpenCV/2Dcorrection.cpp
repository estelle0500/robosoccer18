//#include <opencv2/core/core.hpp>
//#include <opencv2/imgcodecs.hpp>
//#include <opencv2/highgui/highgui.hpp>
//#include <cstring>
//#include <opencv2\imgproc.hpp>
//#include "Calibration.h"
//#include <iostream>
//#include "2Dcorrection.h"
//
//using namespace cv;
//using namespace std;
//
////bool CALIB = true;
//
//double contractionRatio = 0.8;
//
//string IntToString(int a)
//{
//	ostringstream temp;
//	temp << a;
//	return temp.str();
//}
//
//Point getUndistortedPosition(double angle, double r)
//{
//	return Point(round(1000 * cos(angle*3.1415926 / 180))*r+2000,round(1000 * sin(angle*3.1415926 / 180))*r+2000);
//}
//
//Point2f* vector2Point2f(vector<Point> input)
//{
//	Point2f output[4];
//	cout << "conversion\n";
//	for (int i = 0; i < 4; i++)
//	{
//		output[i] = Point2f((float)input[i].x, (float)input[i].y);
//		cout << input[i] << " " << output[i] << "\n";
//	}
//		
//	return output;
//}
//
//Mat findTransformationMatrix(vector<Point> distorted)
//{
//	vector<Point> undistorted;
//	undistorted.push_back(getUndistortedPosition(10,1));
//	undistorted.push_back(getUndistortedPosition(30,1));
//	undistorted.push_back(getUndistortedPosition(60,1));
//	undistorted.push_back(getUndistortedPosition(90,1));
//	cout << undistorted;
//
//	Point2f distorted2[4];
//	Point2f undistorted2[4];
//	cout << "conversion\n";
//	for (int i = 0; i < 4; i++)
//	{
//		distorted2[i] = Point2f((float)distorted[i].x, (float)distorted[i].y);
//		undistorted2[i] = Point2f((float)undistorted[i].x, (float)undistorted[i].y);
//		cout << distorted2[i]<<" "<<undistorted2[i]<<"\n";
//	}
//
//	Mat ans = getPerspectiveTransform(undistorted2, distorted2);
//	//cout << mapPoint(ans, undistorted[0]) << "look";
//	return ans; // seems to be working decently
//}
//
//Point mapPoint(Mat tMatrix, Point originalPos)
//{
//	double x1 = tMatrix.at < double > (0, 0) * originalPos.x + tMatrix.at<double>(0, 1) * originalPos.y + tMatrix.at<double>(0, 2);
//	double x2 = tMatrix.at<double>(1, 0) * originalPos.x + tMatrix.at<double>(1, 1) * originalPos.y + tMatrix.at<double>(1, 2);
//	double x3 = tMatrix.at<double>(2, 0)* originalPos.x + tMatrix.at<double>(2, 1) * originalPos.y + tMatrix.at<double>(2, 2);
//	return Point(round(x1 / x3), round(x2 / x3));
//}
//
//void findTransformationMatrix(Mat& leftMatrix, Mat& rightMatrix, vector<Point> points)  // find the matrix for left and right
//{
//	vector<Point> leftPoints, rightPoints;
//	int centerPos = 0;
//	for (int i = 0; i < 8; i++)
//		centerPos += points[i].x;
//	centerPos = centerPos / 8;
//	cout << "centerPos:" << centerPos << "/n";
//	
//	//divide the points
//	for (int i = 0; i < 8; i++)
//	{
//		if (points[i].x <= centerPos)
//		{
//			leftPoints.push_back(points[i]);
//			cout << "left point:" << points[i] << "/n";
//		}
//		else
//		{
//			rightPoints.push_back(points[i]);
//			cout << "right point:" << points[i] << "/n";
//		}
//	}
//	// safety check
//	if ((leftPoints.size() != 4) || (rightPoints.size() != 4))
//	{
//		cout << "ERROR, Points not balanced\n";
//		getchar();
//		return;
//	}
//
//	leftMatrix = findTransformationMatrix(leftPoints);
//	rightMatrix = findTransformationMatrix(rightPoints);
//}
//
//void markAngles(Mat inputImage, Mat leftMatrix, Mat rightMatrix)
//{
//	// mark center
//	Point center = mapPoint(leftMatrix, getUndistortedPosition(0,0));
//	cout << "center:" << center << "/n";
//	drawMarker(inputImage, center, Scalar(255, 0, 0), 0, 40, 5, 8);
//	center = mapPoint(rightMatrix, getUndistortedPosition(0, 0));
//	cout << "center:" << center << "/n";
//	drawMarker(inputImage, center, Scalar(255, 0, 0), 0, 40, 5, 8);
//
//	for (int i = 0; i < 360; i += 10)
//	{
//		drawMarker(inputImage, mapPoint(leftMatrix, getUndistortedPosition(i, contractionRatio)), Scalar(0, 255, 0), 0, 40, 5, 8);
//		putText(inputImage, IntToString(i), mapPoint(leftMatrix, getUndistortedPosition(i, contractionRatio)), FONT_HERSHEY_PLAIN, 2, Scalar(0, 0, 0, 0),3);
//		drawMarker(inputImage, mapPoint(rightMatrix, getUndistortedPosition(i, contractionRatio)), Scalar(0, 255, 0), 0, 40, 5, 8);
//		putText(inputImage, IntToString(i), mapPoint(rightMatrix, getUndistortedPosition(i, contractionRatio)), FONT_HERSHEY_PLAIN, 2, Scalar(0, 0, 0, 0),3);
//	}
//	showImage(inputImage);
//	//if (CALIB)
//		waitKey(0);
//}
//
//
//// as seen on the imgae, not as shown in the coordinate
//bool isAboveLine(Point leftEnd, Point rightEnd, Point POC)
//{
//	double gradient = (double)(leftEnd.y - rightEnd.y) / (double)(leftEnd.x - rightEnd.x);
//	double yOnline = (double)(leftEnd.y) + gradient*(double)(POC.x - leftEnd.x);
//	if (yOnline > (double)POC.y)
//		return true;
//	else
//		return false;
//}
//
//
//// find the angle of the point using binary search
//// will only search in the 270 to 180 range
//int measureAngle(Mat leftMatrix, Mat rightMatrix, Point point)
//{
//	int upper_bound = 270;
//	int lower_bound = 180;
//	for (int i = 1; i < 7; i++)
//	{
//		int mid = (upper_bound + lower_bound) / 2;
//		if (isAboveLine(mapPoint(leftMatrix, getUndistortedPosition(mid, contractionRatio)), mapPoint(rightMatrix, getUndistortedPosition(mid, contractionRatio)), point))
//			lower_bound = mid;
//		else upper_bound = mid;
//	}
//	return (upper_bound + lower_bound) / 2;
//}