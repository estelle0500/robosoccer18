#include <robosoccer.h>

extern Motor ne(4,47,3,2,true); //D6
extern Motor sw(10,48,9,8,true); //D5
extern Motor nw(7,49,6,5,false); //D4
extern Motor se(28,50,30,29,true); //D3
Compass compass(Serial4, 115200);

Adafruit_VL53L0X ir_left = Adafruit_VL53L0X(), ir_front = Adafruit_VL53L0X(), ir_right = Adafruit_VL53L0X(), ir_back = Adafruit_VL53L0X();

Adafruit_VL53L0X *ir_sensors[4] = {&ir_left, &ir_front, &ir_right, &ir_back};

void setup() {
  // put your setup code here, to run once:

  Serial.begin(9600);
  Wire1.begin();
  delay(5000);
  compass.init();
  for (int i=0; i<20; i++) {
    Serial.println("begin");
    delay(100);
  }
  while (!ir_left.begin(0x70, 7)) {
    Serial.println(F("Failed to boot VL53L0X on port 7"));
    delay(1000);
  }
  while (!ir_right.begin(0x70, 2)) {
    Serial.println(F("Failed to boot VL53L0X on port 7"));
    delay(1000);
  }
  while (!ir_front.begin(0x70, 4)) {
    Serial.println(F("Failed to boot VL53L0X on port 7"));
    delay(1000);
  }
}

void loop() {
  // put your main code here, to run repeatedly:
  // Serial.println("new measurement");
  /*VL53L0X_RangingMeasurementData_t m[4];
  for (int i=0; i<3; i++) {
    ir_sensors[i]->rangingTest(&m[i], false);
    Serial.print(m[i].RangeMilliMeter);
    Serial.print("\t");
  }
  Serial.println();*/
  testCircle(compass);
}
