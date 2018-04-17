#include <robosoccer.h>

void setup() {
  // put your setup code here, to run once:
  setupComponents();
  CURR_SPD = 70;
}

void loop() {
  // put your main code here, to run repeatedly:
  communication.readData();
  int ind = communication.availableList();
  ball_detected = communication.dataSet[ind].cnt != 0; // at least one object detected
  if (ball_detected)
  {
    no_ball_count = 0;
    ball_angle = communication.dataSet[ind].detectedObjectList->angle;
    ball_distance = communication.dataSet[ind].detectedObjectList->distance;
    int angle2 = decideMotionAngle2(ball_angle, ball_distance);
    Serial.print(ball_angle);
    Serial.print("  ");
    Serial.println(angle2);
    DRIVE_DIR = angle2;
    CURR_SPD = 70;
  }
  else if (no_ball_count < INFO_EXPIRED_COUNT) {
    no_ball_count++;
  }
  else {
    CURR_SPD = 0;
  }
  delay(50);
}
