#include "MoveBehindBall.h"


// the angle is measured anti-clockwise from the north direction
int decideMotionAngle(int ballAngle)
{
	// constant for tuning here!
	float k = 0.8;

	bool isMirrored = false;
	// mirror right to left
	if (ballAngle < 0)
	{
		isMirrored = true;
		ballAngle = -ballAngle;
	}

	int motionAngle = (int)((float)ballAngle * (1.0 + k));
	if (motionAngle > 180)
		motionAngle = motionAngle - 360;
	if (isMirrored)
		motionAngle = -motionAngle;
	return motionAngle;
}

// the more complicated one with more terms in
int decideMotionAngle1(int ballAngle, int ballDistance)
{
	// constant for tuning
	float k = 0.8; // very hard to explain
	float distToIm = 15.0f; // the distance behind the real ball where the imaginary ball is placed
	float wSink = 50.0f / (float)ballDistance; // weight factor
	float wIm = 1.0f;

	bool isMirrored = false;
	if (ballAngle < 0)
	{
		ballAngle = -ballAngle;
		isMirrored = true;
	}

	// if the ball is almost in front, do no correction
	if (ballAngle < 5)
		return 0;

	// the point sink part
	Vector vSink = Vector(90 + ballAngle, 1.0);
	// the ball attraction part
	Vector vBall = Vector(90 + (int)((float)ballAngle * (1.0 + k)), 1.0);
	// the imaginary ball attraction
	// note that this function returns in rad
	float imBallAngle = SpeedTrig.atan2((float)ballDistance*SpeedTrig.sin(ballAngle - 90) + distToIm, (float)ballDistance*SpeedTrig.cos(ballAngle - 90));
	imBallAngle = imBallAngle / 3.1415926 * 180.0 + 90.0;
	Vector vImBall = Vector(90 + (int)((float)imBallAngle * (1.0 + k)), 1.0);
	vBall.add(vSink.mWeight(wSink));
	vBall.add(vImBall.mWeight(wIm));

	float motionDirection = SpeedTrig.atan2(-vBall.y, -vBall.x);
	motionDirection = motionDirection * 180 / 3.1415926 + 90;
	if (isMirrored)
		motionDirection = -motionDirection;

	return (int)motionDirection;
}

int decideMotionAngle2(int ballAngle, int ballDistance)
{
	float wSink = 50.0f / (float)ballDistance; // weight factor
	bool isMirrored = false;
	float motionDirection = 0.0;
	if (ballAngle < 0)
	{
		isMirrored = true;
		ballAngle = -ballAngle;
	}

	// meaning that the robot is in front of the ball
	if (ballAngle > 90)
	{
		// need to convert to the standard cartesian angle
		Vector vCircle = Vector(ballAngle + 180, 1.0f);
		Vector vSink = Vector(90 + ballAngle, 1.0f);

		vCircle.add(vSink.mWeight(wSink));
		motionDirection = SpeedTrig.atan2(-vCircle.y, -vCircle.x);
		motionDirection = motionDirection * 180 / 3.1415926 + 90;
	}
	else
	{
		motionDirection = decideMotionAngle1(ballAngle, ballDistance);
	}
	if (isMirrored)
		motionDirection = -motionDirection;

	return (int)motionDirection;
}


// print out the angle only
void mockTesting()
{
	// declare and start the communication
	RpiCommunication communication;
	delay(100);
	for (int i = 0; i < 1000; i++)
	{
		communication.readData();
		int ind = communication.availableList();
		if (communication.dataSet[ind].cnt != 0)
		{
			int angle1 = communication.dataSet[ind].detectedObjectList->angle;
			int angle2 = decideMotionAngle2(angle1,1);
			Serial.print(angle1);
			Serial.print("  ");
			Serial.println(angle2);
		}
		delay(50);
	}
}


