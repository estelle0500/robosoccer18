#include <robosoccer.h>

extern Motor se(4,47,3,2,false); //D6
extern Motor ne(10,48,9,8,false); //D5
extern Motor sw(7,49,6,5,false); //D4
extern Motor nw(28,50,30,29,false); //D3
extern Compass compass(Serial4, 115200);
extern int DRIVE_DIR = 0;
extern int CURR_SPD = 0;

IRSensor ir_front(4,0x70), ir_back(0,0x70), ir_left(7,0x70), ir_right(2,0x70);

extern IRSensor *ir_sensors[4] = {&ir_left, &ir_front, &ir_right, &ir_back};

IntervalTimer updateTimer;

void setup() {
  // put your setup code here, to run once:
  Wire1.begin();
  for (int i=0; i<4; i++) ir_sensors[i]->init();
  compass.init();
  Serial.begin(115200);
  updateTimer.begin(updateAll, INTERVAL);
}

void waitKey() {
  // type q to stop the robot from running
  // type r to restart the robot
  while (Serial.available()) {
    byte b = Serial.read();
    if (b == 'q') {
      bool restarted = false;
      while (!restarted) {
        if (Serial.available()) {
          byte b = Serial.read();
          if (b == 'r') restarted=true;
        }
      }
    }
  }
}

void loop() {
  //Serial.println(compass.getAngle());

  //drive forward
  DRIVE_DIR = 0;
  CURR_SPD = 50;
  delay(5000);
  //stop
  CURR_SPD = 0;
  delay(1000);
  //drive backward
  DRIVE_DIR = 180;
  CURR_SPD = 50;
  delay(5000);
  //stop
  CURR_SPD = 0;
  delay(1000);
  waitKey();
}
