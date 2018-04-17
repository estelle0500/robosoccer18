#ifndef MOVEMENT_H
#define MOVEMENT_H

#include "../components/compass.h"
#include "../components/motor.h"
#include "../external_libs/SpeedTrig/SpeedTrig.h"

#define ALIGN_GAIN_C 0.9
#define ALIGN_GAIN_M 0.003
#define DRIVE_OFFSET 20
#define MAX_TURN 150
#define MAX_SPD 60
extern volatile int DRIVE_DIR, CURR_SPD;

extern Motor se, ne, sw, nw;

/* Initialize motors and compass */
// void init_drive(float *north);

/*
Compute the speed of each motor based on the direction the Robot needs to move in.

@param drvDir The direction the Robot should move in (-180deg to 180deg)
@param maxSpd Max speed the Robot can move at
@param currAngle Current heading of the Robot
@param alignAngle The direction of 'North'
@param maxTurn Max angular speed the Robot can move at
*/
void computeDrive(float drvDir, float maxSpd, float currAngle, float alignAngle, float maxTurn, int speeds[4]);

/* Run the motors at the computed speed */
void vroom(int speeds[]);

/* Stop all motors */
void brake();

/* Miscellaneous helper function */
inline int sign(float a){
  if(a>0) return 1;
  else if(a<0) return -1;
  return 0;
}

inline int sign(int a){
  if(a>0) return 1;
  else if(a<0) return -1;
  return 0;
}
#endif
