#include "ultrasound_sensor.h"

UltrasoundSensor::UltrasoundSensor(int _pin, Adafruit_ADS1115& _ads, float _scaling_factor):
pin(_pin), ads(_ads), scaling_factor(_scaling_factor) {

}

void UltrasoundSensor::readDistance() {
    int adc = ads.readADC_SingleEnded(pin);
    this->distance = adc*scaling_factor;
}
