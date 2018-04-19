#include <robosoccer.h>

void setup() {
  // put your setup code here, to run once:
  setupComponents();
}

void loop() {
  // put your main code here, to run repeatedly:
  communication.readData();
  int ind = communication.availableList();
  ball_detected = communication.dataSet[ind].cnt != 0;
  if (ball_detected) {
    ball_angle = communication.dataSet[ind].detectedObjectList->angle;
    ball_distance = communication.dataSet[ind].detectedObjectList->distance;
    DRIVE_DIR = decideMotionAngle2(ball_angle, ball_distance);
    CURR_SPD = 60;
  }
}
