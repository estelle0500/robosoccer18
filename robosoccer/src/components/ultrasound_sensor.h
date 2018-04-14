#ifndef ULTRASOUND_SENSOR_H
#define ULTRASOUND_SENSOR_H

#include "../external_libs/Adafruit_ADS1X15/Adafruit_ADS1015.h"

class UltrasoundSensor {
    Adafruit_ADS1115& ads;
    float scaling_factor;
    volatile float distance;
    int pin;
public:
    UltrasoundSensor(int _pin, Adafruit_ADS1115& _ads, float _scaling_factor=1.0);
    void readDistance();
    inline float getDistance() {
        return this->distance;
    }
};

#endif
