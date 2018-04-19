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
    if (abs(ball_angle)<=5) {
      DRIVE_DIR = 0;
      looping_direction = NO_DIRECTION;
    }
    else if (abs(ball_angle)<=90) {
      DRIVE_DIR = decideMotionAngle1(ball_angle, ball_distance);
      looping_direction = NO_DIRECTION;
    }
    else if (abs(ball_angle)>=160) {
      if (looping_direction != NO_DIRECTION) {
        if (blocked_dir[LEFT]) looping_direction = CLOCKWISE;
        else if (blocked_dir[RIGHT]) looping_direction = ANTICLOCKWISE;
        else if (ball_angle > 0) looping_direction = ANTICLOCKWISE;
        else looping_direction = CLOCKWISE;
      }
      DRIVE_DIR = decideMotionAngle2(ball_angle, ball_distance, looping_direction);
    }
    else {
      if (ball_angle > 0) looping_direction = ANTICLOCKWISE;
      else looping_direction = CLOCKWISE;
      DRIVE_DIR = decideMotionAngle2(ball_angle, ball_distance, looping_direction);
    }
    Serial.print(ball_angle);
    Serial.print("\t");
    Serial.println(DRIVE_DIR);
    CURR_SPD = 70;
  }
  else if (no_ball_count < INFO_EXPIRED_COUNT) {
    no_ball_count++;
  }
  else {
    has_ball = false;
    CURR_SPD = 0;
  }
  delay(50);
}
