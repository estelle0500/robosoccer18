#ifndef COMPASS_H

/* Read angle from compass using Serial */
float readAngle();

/* Calculate how much the Robot should turn by.
PID is applied to dampen oscillation of Robot.

@param val
@param set
@param gain
@param maximum
@param offset
@param sqFactor
*/
int anglePid(float val,float set, float gain, int maximum, int offset=20, int sqFactor=70);

#endif