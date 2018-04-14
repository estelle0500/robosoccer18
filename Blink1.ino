#include "RpiCommunication.h"
#include "MoveBehindBall.h"

RpiCommunication communication;

// the setup function runs once when you press reset or power the board
void setup() {
  // initialize digital pin 13 as an output.
  pinMode(13, OUTPUT);
  //communication.readData();

}

// the loop function runs over and over again forever
void loop() {
	mockTesting();
	delay(10000000);
}
