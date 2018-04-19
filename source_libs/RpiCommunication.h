#ifndef RPICOMMUNICATION_H
#define RPICOMMUNICATION_H

#define MAX_NUMBER_OF_OBJECTS 15
#include "Arduino.h"

/* Start up compass */
class RpiCommunication {
private:
	int baud_rate;
	int internalBuffer[128];
	int internalBufferCnt;
public:
	/* Declare the serial port used for Rpi communication*/
	RpiCommunication();

	struct detectedObject
	{
		//color ind
		//red = 0
		//blue = 1
		//purple = 2
		//yellow = 3
		//green = 4
		// orange = 5 for the ball
		detectedObject() {}
		detectedObject(int _identifier, int _angle, int _distance)
		{
			identifier = _identifier;
			angle = _angle;
			distance = _distance;
		}
		int identifier;
		int angle; // -180 to 180, measured anti-clockwise from the north
		int distance; // in cm, measured from the center of the robot
	};

	struct incomingData
	{
		int cnt;
		detectedObject detectedObjectList[MAX_NUMBER_OF_OBJECTS];
		incomingData() { clear(); }
		void clear()
		{
			cnt = 0;
		}
		int length()
		{
			return cnt;
		}
		void add(detectedObject newObject)
		{
			detectedObjectList[cnt] = newObject;
			cnt++;
		}
	};

	/*
	Protocol for communication
	RPi will use 255 to signal the start of message and 254 to signal the end of message
	The marker might be sent for more than one times to prevent loss in during transmission
	The data will only use 0 - 199, and the rest are for marking/instruction
	RPi will send the the identifier of each of the object detected
	angle (in deg), distance / 2 (in cm)
	For each of the dectected object,
	the fisrt byte encodes identifier and the sign of the angle (200 + identifier + 10 (+) or 0 (-) )
	then, angle (0 - 180)
	then, distance / 2 (0 - 199)
	*/
	incomingData dataSet[2];
	int currentlyReceivingSet = 0;
	bool currentSetComplete = false;
	void readData();
	void updateBuffer();
	void printOutAll();
	void flush();
	void printOut(detectedObject object);
	int availableList();
};
#endif

