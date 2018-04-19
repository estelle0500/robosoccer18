#include "RpiCommunication.h"

RpiCommunication::RpiCommunication() 
{
	Serial.begin(115200);
	flush();
}

void RpiCommunication::updateBuffer()
{
	internalBufferCnt = 0;
	while (Serial.available())
	{
		internalBuffer[internalBufferCnt] = Serial.read();
		internalBufferCnt++;
	}
}

void RpiCommunication::flush()
{
	while (Serial.available())
		Serial.read();
}

/*
Protocol for communication
RPi will use 255 to signal the start of message and 254 to signal the end of message
The data will only use 0 - 199, and the rest are for marking/instruction
RPi will send the angle (in deg), distance / 2 (in cm), and the identifier of each of the object detected
For each of the dectected object,
the fisrt byte encodes identifier and the sign of the angle (200 + identifier + 10 (+) or 0 (-) )
then, angle (0 - 180)
then, distance / 2 (0 - 199)
*/

void RpiCommunication::readData()
{
	updateBuffer();
	int i = 0;
	while (i < internalBufferCnt)
	{
		// scan for indicators of the start of a detected object
		while (internalBuffer[i] < 200)
		{
			i++;
			continue;
		}

		// does not matter how many 254 there are. Will just set this repeatedly. No additional effect
		if (internalBuffer[i] == 254)
		{
			currentSetComplete = true;
			i++;
			continue;
		}

		// matters for any repeats. Will need to fitler it out
		if (internalBuffer[i] == 255)
		{
			// scan past all the 255 sent
			while (internalBuffer[i] == 255 && i < internalBufferCnt)
				i++;
			// start a new set
			currentSetComplete = false;
			currentlyReceivingSet = 1 - currentlyReceivingSet;
			dataSet[currentlyReceivingSet].clear();
			continue;
		}

		// if a normal data point comes in
		// check if there is still enough data. If not enough data, end of the story
		if (i + 2 >= internalBufferCnt)
			break;
		// check if the data is complete
		if (internalBuffer[i + 1] > 200 || internalBuffer[i + 2] > 200)
		{
			i++;
			continue;
		}
		// retrieve angle
		int angle = internalBuffer[i + 1];
		// put in the sign for the angle
		if ((internalBuffer[i] / 10) % 10 == 0)
			angle = -angle;
		int distance = internalBuffer[i + 2] * 2;
		int identifier = (internalBuffer[i] % 10);
		dataSet[currentlyReceivingSet].add(detectedObject(identifier, angle, distance));
		i = i + 3;
	}
}

void RpiCommunication::printOut(detectedObject object)
{
	Serial.print("identifier:");
	Serial.print(object.identifier);
	Serial.print("angle:");
	Serial.print(object.angle);
	Serial.print("distance:");
	Serial.println(object.distance);
	Serial.println();
}

void RpiCommunication::printOutAll()
{
	Serial.println("currentList");
	for (int i = 0; i < dataSet[currentlyReceivingSet].cnt; i++)
		printOut(dataSet[currentlyReceivingSet].detectedObjectList[i]);
	Serial.println("bufferedList"); 
	for (int i = 0; i < dataSet[1-currentlyReceivingSet].cnt; i++)
		printOut(dataSet[1-currentlyReceivingSet].detectedObjectList[i]);
}

int RpiCommunication::availableList()
{
	if (currentSetComplete)
		return currentlyReceivingSet;
	else return 1 - currentlyReceivingSet;
}