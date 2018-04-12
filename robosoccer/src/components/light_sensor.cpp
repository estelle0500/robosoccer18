#include <Arduino.h>
#include "light_sensor.h"
#include "../logic/movement.h"


// which direction to move in
// e.g. line detected on front sensor -> move backwards
int deg_directions[4] = {180, -90, 0, 90};
volatile unsigned long last_interrupted = 0;

LightSensor::LightSensor(uint8_t pin_number, uint8_t direction) {
    this->pin_number = pin_number;
    this->direction = direction;
}

void LightSensor::init(bool interrupt_enabled) {
    pinMode(pin_number, INPUT);
    if (interrupt_enabled) {
        switch(direction){
            case DIR_FRONT: attachInterrupt(digitalPinToInterrupt(pin_number), avoidLineFront, RISING);
            case DIR_RIGHT: attachInterrupt(digitalPinToInterrupt(pin_number), avoidLineRight, RISING);
            case DIR_BACK: attachInterrupt(digitalPinToInterrupt(pin_number), avoidLineBack, RISING);
            case DIR_LEFT: attachInterrupt(digitalPinToInterrupt(pin_number), avoidLineLeft, RISING);
        }
    }
}

void LightSensor::avoidLine(uint8_t direction) {
    if (millis()-last_interrupted < MIN_INTERRUPT_INTERVAL) {
        return; // to prevent too frequent interrupts
    }
    CURR_SPD = MAX_SPD;
    DRIVE_DIR = deg_directions[direction];
    last_interrupted = millis();
}

void LightSensor::avoidLineFront() {
    cli();
    avoidLine(DIR_FRONT);
    sei();
}

void LightSensor::avoidLineRight() {
    cli();
    avoidLine(DIR_RIGHT);
    sei();
}

void LightSensor::avoidLineBack() {
    cli();
    avoidLine(DIR_BACK);
    sei();
}

void LightSensor::avoidLineLeft() {
    cli();
    avoidLine(DIR_LEFT);
    sei();
}
