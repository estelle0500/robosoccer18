#include "SensorModel.h"
//#include "fastRNG.h"
void setup() {
  Serial.begin(115200);
  Serial.println("starting");
}
//fastRNG generator;
void loop() {
//  float R1,R2;
//  generator.gaussian(R1,R2,10);
//  Serial.print(R1);
//  Serial.print(" ");
//  Serial.println(R2);
//  delay(1000);
  SensorModel sensorModel;
  sensorModel.probFieldInit();
  unsigned long t1 = micros();
  SensorModel::sensorReading reading(79, 79,150 , 73);//99
  sensorModel.calProbabilityField(reading);
  unsigned long t2 = micros();
  robotPose pose = sensorModel.mostLikelyPose;
  Serial.println(t2-t1);
  Serial.print(pose.x);
  Serial.print(" ");
  Serial.println(pose.y);
  delay(1000);
 }
