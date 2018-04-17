#include "movement_tests.h"

void testTurn(Compass& compass, float target){
  float angle=compass.readAngle();
  Serial.println(angle);
  int turnSpd=anglePid(angle,target,0.6,50);
  int speeds[4];
  speeds[0]=turnSpd;
  speeds[1]=turnSpd;
  speeds[2]=turnSpd;
  speeds[3]=turnSpd;
  vroom(speeds);
}

void testPattern(){
  int i=-180;
  int speeds[4]={0,0,0,0};
  while(true){
    computeDrive(i,60,10,10,MAX_TURN, speeds);
    vroom(speeds);
    delay(500);
    brake();
    delay(1000);
    computeDrive(i,-60,10,10,MAX_TURN, speeds);
    vroom(speeds);
    delay(500);
    brake();
    delay(1000);
//    i+=45;
//    i=i%360;
  }
}

void testAlignStatic(Compass& compass, float turn){
  float angle = compass.readAngle();
  float north = compass.getNorth();
  int speeds[4]={0,0,0,0};
  computeDrive(turn+north-angle,50,angle,north,150, speeds);
  Serial.print(turn+north-angle);Serial.print("\t");Serial.print(speeds[0]);Serial.print("\t");Serial.print(speeds[1]);Serial.print("\t");Serial.print(speeds[2]);Serial.print("\t");Serial.println(speeds[3]);
//  vroom();
}

void testAlign(Compass& compass, float turn){
  long t0=millis();
  float angle;
  float north = compass.getNorth();
  int speeds[4]={0,0,0,0};
  while(millis()-t0 < 600){
    angle=compass.readAngle();
    computeDrive(turn+north-angle, 100, angle, north, MAX_TURN, speeds);
    vroom(speeds);
  }
  brake();
  delay(1000);
  t0=millis();
  while(millis()-t0 < 600){
    angle=compass.readAngle();
    computeDrive(turn +north-angle+180, 100, angle, north, MAX_TURN, speeds);
    vroom(speeds);
  }
  brake();
  delay(4000);
//    i+=45;
//    i=i%360;
}

void testCircle(Compass& compass){
  int i=0; //bearing wrt N
  long t0=millis();
  float angle;
  float north=compass.getNorth();
  int speeds[4]={0,0,0,0};
  while(millis()-t0<6000){
    angle=compass.readAngle();
    computeDrive(i+north-angle,150,angle,north,MAX_TURN,speeds);
    vroom(speeds);
    i+=5;
    i=i%360;
    delay(5);
  }
  brake();
  delay(3000);
}

void testMotors(){
  nw.move(60);
  delay(500);
  nw.move(-60);
  delay(500);
  brake();
  delay(500);

  ne.move(60);
  delay(500);
  ne.move(-60);
  delay(500);
  brake();
  delay(500);

  se.move(60);
  delay(500);
  se.move(-60);
  delay(500);
  brake();
  delay(500);

  sw.move(60);
  delay(500);
  sw.move(-60);
  delay(500);
  brake();
  delay(500);
}

void testRun(){
    int speeds[4];
  speeds[0]=100;
  speeds[1]=100;
  speeds[2]=100;
  speeds[3]=100;
  vroom(speeds);
}
