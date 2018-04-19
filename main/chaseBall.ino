#include <movement.h>
#include <movement_tests.h>
extern Motor ne(4,47,3,2,true); //D6
extern Motor nw(10,48,9,8,true); //D5
extern Motor sw(7,49,6,5,false); //D4
extern Motor se(28,50,30,29,true); //D3

void setup() {
  // put your setup code here, to run once:
  Serial.println("BEGIN");
  Serial.begin(115200);
}

int arr[5];
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

  bool data;
  while (Serial.available()){
    byte input = Serial.read();
    if (input == 254){
      arr[0] = 254;
      arr[1] = Serial.read();
      arr[2] = Serial.read();
      arr[3] = Serial.read();
      arr[4] = Serial.read();
      if (arr[4] == 255){
        data = true;
        break;
      }
      else{
        data = false;
        break;
      }
    }
  }
  if (data == true){
    CURR_SPD = 50;
    if (arr[2] == 0){
      arr[3] = -arr[3];
    }
    if (arr[2] == 1){
      arr[3] = arr[3];
    }
    DRIVE_DIR = arr[3];
  }
  else{
    CURR_SPD = 0;
  }
}
