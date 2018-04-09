// include whatever code that is needed here, in the form of header file with libraries
// call the function in the main function
#include <cstring>
#include <iostream> 
#include <fstream>
#include "fastRNG.h"
#include "ParticleFilter.h"
#include "Stats.h"
#include "opencv2/highgui/highgui.hpp"
using namespace std;
using namespace cv;

ofstream ffout("C:/Users/Zhang Yansheng/Desktop/temp.txt");

void printCompatible(double number)
{
	cout << "Internal`StringToDouble[\"" << number << "\"]";
}

void printCompatible2(double number)
{
	ffout << "Internal`StringToDouble[\"" << number << "\"]";
}

int main()
{
	SensorModel sensorModel;
	sensorModel.probFieldInit(30, 30);
	sensorModel.calProbabilityField(SensorModel::sensorReading(89, 79, 89, 73));
	ffout << "ListPlot3D[{";
	for (int i = 0; i < sensorModel.xSize; i++)
		for (int j = 0; j < sensorModel.ySize; j++)
		{
			int x = (int)(i*sensorModel.xWidth + sensorModel.xWidth / 2.0);
			int y = (int)(j*sensorModel.yWidth + sensorModel.yWidth / 2.0);
			ffout << "{" << x << "," << y << ",";
			printCompatible2(sensorModel.probField[i][j]);
			ffout << "},";
		}
	ffout << "{0,0,0}}]";
	ffout.close();
	char c;
	getchar();
	return 0;

#ifdef VISUALIZATION_ENABLED



	ParticleFilter filter(1000);
	//filter.sensorReadingProb(0, 80, ParticleFilter::element(robotPose(60, 90, 0),1));
	filter.resetDisplay();
	filter.showImage("Test!");
	filter.draw(filter.currentParticles,'R');
	filter.showImage("Test!");
	filter.update(50, 0, SensorModel::sensorReading(86, 83, 89, 83));
	filter.draw(filter.currentParticles, 'G');
	filter.showImage("Test!");
	//filter.printOutAll(0);
	//cout << "ListPlot[{";
	//for (int i = 0; i < 1000; i++)
	//{
	//	ParticleFilter::element pose = filter.motionSampler(ParticleFilter::element(robotPose(0, 0, 0),1), 10, 45);

	//	cout << "{"<<pose.pose.x<<","<<pose.pose.y << "},";
	//}
	//cout << "}]";
	destroyAllWindows();
	char c;
	cin >> c;
	return 0;
	/*initROI(image);
	extractPatternPoints(image);*/

	/*vector<Point> points = selectBlue(image);
	cout << points.size() << "\n";

	// check if precisely 8 points are detected.
	if (points.size() != 8)
	{
		cout << "ERROR! CHECK CALIBRATION/n";
		getchar();
		return 0;
	}

	Mat leftM, rightM;
	findTransformationMatrix(leftM, rightM, points);
	markAngles(image, leftM, rightM);

	drawMarker(image, Point(1000, 700), Scalar(0, 255, 0), 0, 40, 5, 8);
	showImage(image);
	waitKey(0);
	int angle = measureAngle(leftM, rightM, Point(1000, 700));
	
	putText(image, IntToString(angle), Point(1000,700), FONT_HERSHEY_PLAIN, 2, Scalar(0, 0, 0, 0), 3);

	angle = measureAngle(leftM, rightM, Point(1000, 550));
	drawMarker(image, Point(1000, 550), Scalar(0, 255, 0), 0, 40, 5, 8);
	putText(image, IntToString(angle), Point(1000, 550), FONT_HERSHEY_PLAIN, 2, Scalar(0, 0, 0, 0), 3);
	showImage(image);
	waitKey(0);
	*/


	//Mat sth = findTransformationMatrix(points);
	//cout << sth;
	//Point center = mapPoint(sth, Point(2000, 2000));
	//drawMarker(image, center, Scalar(255, 0, 0), 0, 40, 5, 8);
	////drawMarker(image, Point(100,100), Scalar(255, 0, 0), 0, 40, 5, 8);
	//cout << "center\n" << center;
	//for (int i = 10; i < 190; i += 10)
	//{
	//	drawMarker(image, mapPoint(sth, getUndistortedPosition(i,1)), Scalar(0, 255, 0), 0, 40, 5, 8);
	//	putText(image, IntToString(i), mapPoint(sth, getUndistortedPosition(i,1)), FONT_HERSHEY_PLAIN, 2, Scalar(0, 0, 255, 255));
	//}
	//
	//showImage(image);
	//waitKey(0);
	return 0;
#endif // VISUALIZATION_ENABLED
}
