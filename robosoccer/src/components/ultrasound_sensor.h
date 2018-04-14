#ifndef ULTRASOUND_SENSOR_H
#define ULTRASOUND_SENSOR_H

extern Adafruit_ADS1115 ads;
class UltrasoundSensor {
    float scaling_factor;
    int pin;
public:
    UltrasoundSensor(int _pin, float _scaling_factor=1.0): pin(_pin), scaling_factor(_scaling_factor) {};
    float readDistance() {
        int adc = ads.readADC_SingleEnded(0);
        return adc*scaling_factor;
    }
};

#endif
