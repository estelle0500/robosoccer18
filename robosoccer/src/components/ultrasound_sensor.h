#ifndef ULTRASOUND_SENSOR_H
#define ULTRASOUND_SENSOR_H

#include "../external_libs/Adafruit_ADS1X15/Adafruit_ADS1015.h"

class UltrasoundSensor {
    int pin;
    Adafruit_ADS1115& ads;
    float scaling_factor;
    volatile float distance;
public:
    UltrasoundSensor(int _pin, Adafruit_ADS1115& _ads, float _scaling_factor=0.0343);
    void readDistance();
    inline float getDistance() {
        return this->distance;
    }
};

#endif
