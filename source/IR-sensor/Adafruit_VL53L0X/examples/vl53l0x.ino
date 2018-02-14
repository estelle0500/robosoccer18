#include "Adafruit_VL53L0X.h"

Adafruit_VL53L0X lox = Adafruit_VL53L0X();
Adafruit_VL53L0X lox2 = Adafruit_VL53L0X();

void setup() {
  Serial.begin(115200);

  // wait until serial port opens for native USB devices
  while (! Serial) {
    delay(1);
  }

  Wire.begin();
  Serial.println("Adafruit VL53L0X test");
  // lox.begin(MULTIPLEXER_ADDRESS, I2C_CHANNEL);
  if (!lox.begin(0x70, 0)) {
    Serial.println(F("Failed to boot VL53L0X"));
    while(1);
  }

  if (!lox2.begin(0x70, 1)) {
    Serial.println(F("Failed to boot VL53L0X2"));
    while(1);
  }
  // power 
  Serial.println(F("VL53L0X API Simple Ranging example\n\n")); 
}


void loop() {
  uint8_t isReady = 0, isReady2 = 0;
  VL53L0X_RangingMeasurementData_t measure, measure2;
  lox.CheckMeasurementDataReady(&isReady);
  lox2.CheckMeasurementDataReady(&isReady2);
  if (isReady = 0x01) {
    lox.GetRangeMeasurement(&measure);
    if (measure.RangeStatus != 4) {  // phase failures have incorrect data
      Serial.println(measure.RangeMilliMeter);
    } 
  }
  if (isReady2 = 0x01) {
    lox2.GetRangeMeasurement(&measure2);
    if (measure.RangeStatus != 4) {  // phase failures have incorrect data
      Serial.print("    "); Serial.println(measure.RangeMilliMeter);
    } 
  }
  delay(500);
}
