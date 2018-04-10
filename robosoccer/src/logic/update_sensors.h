#ifndef UPDATE_SENSORS_H
#define UPDATE_SENSORS_H

#include "Arduino.h"
#include "../components/compass.h"
#include "../components/ir_sensor.h"
#include "../logic/movement.h"

// interval in microseconds
#define INTERVAL 10000

extern Compass compass;
// extern IntervalTimer updateTimer;

void updateDriving();
void updateAll();

void initTimer();

#endif
