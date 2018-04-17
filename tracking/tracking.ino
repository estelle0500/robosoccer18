#include "Adafruit_VL53L0X.h"
#include <i2c_t3.h>

Adafruit_VL53L0X lox = Adafruit_VL53L0X();
Adafruit_VL53L0X lox2 = Adafruit_VL53L0X();
Adafruit_VL53L0X lox3 = Adafruit_VL53L0X();
Adafruit_VL53L0X lox4 = Adafruit_VL53L0X();

void setup() {
  Serial1.begin(115200);
  time();
  
  while (! Serial1) {
    delay(1);
  }
  
  Serial1.println("Adafruit VL53L0X test");
  while (!lox.begin(0x70, 2)) {                           //check against PCB file
    Serial1.println(F("Failed to boot VL53L0X on port 2"));
    delay(1000);
  }

  while (!lox2.begin(0x70, 6)) {
    Serial1.println(F("Failed to boot VL53L0X on port 6"));
    delay(1000);
  }

  while (!lox3.begin(0x70, 7)) {
    Serial1.println(F("Failed to boot VL53L0X on port 7"));
    delay(1000);
  }

  while (!lox4.begin(0x70, 7)) {
    Serial1.println(F("Failed to boot VL53L0X on port 7"));
    delay(1000);
  }
 
  Serial1.println(F("VL53L0X API Simple Ranging example\n\n")); 
}

void loop() {
  VL53L0X_RangingMeasurementData_t m1, m2, m3, m4;

  Serial1.write("#");
  lox.rangingTest(&m1, false);
  lox2.rangingTest(&m2, false);
  lox3.rangingTest(&m3, false); 
  lox4.rangingTest(&m4, false);

  float temp1=m1.RangeMilliMeter, temp2=m2.RangeMilliMeter, temp3=m3.RangeMilliMeter, temp4=m4.RangeMilliMeter;
  byte * a=(byte *) &temp1;
  byte * b=(byte *) &temp2;
  byte * c=(byte *) &temp3;
  byte * d=(byte *) &temp4;

  Serial1.write(a, 4);
  Serial1.write(b, 4);
  Serial1.write(c, 4);
  Serial1.write(d, 4);
}
