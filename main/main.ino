#include <compass.h>
#include <compass_tests.h>
#include <movement.h>
#include <movement_tests.h>

#include <Wire.h>
#include <WireKinetis.h>

extern Motor ne(4,47,3,2,true); //D6
extern Motor nw(10,48,9,8,true); //D5
extern Motor sw(7,49,6,5,false); //D4
extern Motor se(28,50,30,29,true); //D3
Compass compass(Serial4, 115200);

void setup() {
  // put your setup code here, to run once:
  Serial.println("BEGIN");
  Serial.begin(115200);
  Wire.begin();
  compass.init();
  Serial.println(compass.getNorth());
}

void loop() {
  // put your main code here, to run repeatedly:
  // testMotors();
  testAlign(compass, compass.getNorth());
  // testTurn(compass, 10);
  // Serial.println(compass.readAngle());
}
