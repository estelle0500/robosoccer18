#include <../source/movement.cpp>

void testTurn(){
  float angle=readAngle();
  Serial.println(angle);
  float target=90;
  int turnSpd=anglePid(angle,target,0.6,50);
  speeds[0]=turnSpd;
  speeds[1]=turnSpd;
  speeds[2]=turnSpd;
  speeds[3]=turnSpd;
  vroom();
}

void testPattern(){
  int i=-180;
  while(true){
    computeDrive(i,60,10,10,MAX_TURN);
    vroom();
    delay(500);
    brake();
    delay(1000);
    computeDrive(i,-60,10,10,MAX_TURN);
    vroom();
    delay(500); 
    brake();
    delay(1000);
//    i+=45;
//    i=i%360;
  }
}

void testAlignStatic(){
  int i=0;
  float angle;
  angle=readAngle();
  computeDrive(i+north-angle,50,angle,north,150);
  Serial.print(i+north-angle);Serial.print("\t");Serial.print(speeds[0]);Serial.print("\t");Serial.print(speeds[1]);Serial.print("\t");Serial.print(speeds[2]);Serial.print("\t");Serial.println(speeds[3]);
//  vroom();
}

void testAlign(){
  int i=0;
  long t0=millis();
  float angle;
  while(millis()-t0<600){
    angle=readAngle();
    computeDrive(i+north-angle,200,angle,north,MAX_TURN);
    vroom();
  }
  brake();
  delay(1000);
  t0=millis();
  while(millis()-t0<600){
    angle=readAngle();
    computeDrive(i+north-angle+180,200,angle,north,MAX_TURN);
    vroom();
  }
  brake();
  delay(4000);
//    i+=45;
//    i=i%360;
}

void testCircle(){
  int i=0; //bearing wrt N
  long t0=millis();
  float angle;
  while(millis()-t0<6000){
    angle=readAngle();
    computeDrive(i+north-angle,150,angle,north,MAX_TURN);
    vroom();
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
  speeds[0]=100;
  speeds[1]=100;
  speeds[2]=100;
  speeds[3]=100;
  vroom();
}
