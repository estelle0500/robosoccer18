#include <Motor.h>
#ifndef MOVEMENT_H

#define ALIGN_GAIN_C 0.9
#define ALIGN_GAIN_M 0.003
#define DRIVE_OFFSET 20
#define MAX_TURN 150

Motor se(51,56,3,2,true); //3
Motor ne(52,55,12,11,true); //2
Motor sw(53,54,10,9,true); //4
Motor nw(50,57,7,6,true); //1

/* Initialize motors and compass */
void init_drive();

/* 
Compute the speed of each motor based on the direction the Robot needs to move in.

@param drvDir The direction the Robot should move in (-180deg to 180deg)
@param maxSpd Max speed the Robot can move at
@param currAngle Current heading of the Robot
@param alignAngle The direction of 'North'
@param maxTurn Max angular speed the Robot can move at
*/
void computeDrive(float drvDir, float maxSpd, float currAngle, float alignAngle, float maxTurn);

/* Run the motors at the computed speed */
void vroom();

/* Stop all motors */
void brake();

/* Miscellaneous helper function */
int sign(float a){
  if(a>0) return 1;
  else if(a<0) return -1;
  return 0;
}

int sign(int a){
  if(a>0) return 1;
  else if(a<0) return -1;
  return 0;
}
#endif