#ifndef COMPASS_H
#define COMPASS_H
#include <Arduino.h>

/* Start up compass */
class Compass {
    private:
        HardwareSerial *serial;
        int baud_rate;
        volatile int angle;
        int north;

    public:
        /* Declare compass' serial port */
        Compass(HardwareSerial& serial, int baud_rate);

        /* Start up the compass */
        void init();

        /* Read angle from compass using Serial */
        bool readAngle();

        inline int getNorth() {
          return this->north;
        };

        inline int getAngle() {
            return this->angle;
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
