//
//#include <opencv2/core/core.hpp>
//#include <opencv2/imgcodecs.hpp>
//#include <opencv2/highgui/highgui.hpp>
//
//#include <iostream>
//#include <string>
//
//#include "ReadIm.h"
//
//
//using namespace cv;
//using namespace std;
//
//int mainnn(int argc, char** argv)
//{
//	// be careful about the slash direction. If the slash is in the wrong direction, the compiler with mistake it as escape character
//	string imageName("C:/Users/Zhang Yansheng/Desktop/Sample.jpg"); // by default
//	if (argc > 1)
//	{
//		imageName = argv[1];
//	}
//
//	Mat image;
//	image = imread(imageName.c_str(), IMREAD_COLOR); // Read the file
//
//	if (image.empty())                      // Check for invalid input
//	{
//		cout << "Could not open or find the image" << std::endl;
//		return -1;
//	}
//
//	namedWindow("Display window", WINDOW_AUTOSIZE); // Create a window for display.
//	imshow("Display window", image);                // Show our image inside it.
//
//	waitKey(0); // Wait for a keystroke in the window
//	return 0;
//}
