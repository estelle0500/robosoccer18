#ifndef COMPASS_H
#define COMPASS_H
#include <Arduino.h>

/* Start up compass */
class Compass {
    private:
        HardwareSerial *serial;
        int baud_rate;
        float north;

    public:
        /* Declare compass' serial port */
        Compass(HardwareSerial& serial, int baud_rate);

        void init();

        /* Read angle from compass using Serial */
        float readAngle();

        /* Get north */
        inline float getNorth() {
          return this->north;
        };
};
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
