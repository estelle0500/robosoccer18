#include "Adafruit_VL53L0X.h"
#include <i2c_t3.h>

Adafruit_VL53L0X lox = Adafruit_VL53L0X();
Adafruit_VL53L0X lox2 = Adafruit_VL53L0X();
Adafruit_VL53L0X lox3 = Adafruit_VL53L0X();

void setup() {
  Serial.begin(115200);
  Wire1.begin();

  // wait until serial port opens for native USB devices
  while (! Serial) {
    delay(1);
  }
  
  Serial.println("Adafruit VL53L0X test");
  while (!lox.begin(0x70, 2)) {
    Serial.println(F("Failed to boot VL53L0X on port 2"));
    delay(1000);
  }

  while (!lox2.begin(0x70, 6)) {
    Serial.println(F("Failed to boot VL53L0X on port 6"));
    delay(1000);
  }

  while (!lox3.begin(0x70, 7)) {
    Serial.println(F("Failed to boot VL53L0X on port 7"));
    delay(1000);
  }
  // power 
  Serial.println(F("VL53L0X API Simple Ranging example\n\n")); 
}


void loop() {
  VL53L0X_RangingMeasurementData_t m1, m2, m3;
    
  Serial.print("Reading a measurement... ");
  lox.rangingTest(&m1, false); // pass in 'true' to get debug data printout!
  lox2.rangingTest(&m2, false); // pass in 'true' to get debug data printout!
  lox3.rangingTest(&m3, false); // pass in 'true' to get debug data printout!

  Serial.print(m1.RangeMilliMeter);
  Serial.print("\t");
    Serial.print(m2.RangeMilliMeter);
  Serial.print("\t");
    Serial.println(m3.RangeMilliMeter);
/*
  if (measure.RangeStatus != 4) {  // phase failures have incorrect data
    Serial.print("Distance (mm): "); Serial.println(measure.RangeMilliMeter);
  } else {
    Serial.println(" out of range ");
  }*/
    
  delay(100);
}
