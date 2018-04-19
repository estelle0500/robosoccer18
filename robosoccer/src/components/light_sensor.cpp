#include <Arduino.h>
#include "light_sensor.h"
#include "../logic/movement.h"

LightSensor::LightSensor(uint8_t pin_number, uint8_t direction) {
    this->pin_number = pin_number;
    this->direction = direction;
}

void LightSensor::init() {
    pinMode(pin_number, INPUT);
}
