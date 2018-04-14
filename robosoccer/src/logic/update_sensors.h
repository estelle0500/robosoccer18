#ifndef UPDATE_SENSORS_H
#define UPDATE_SENSORS_H

#include "Arduino.h"
#include "../components/compass.h"
#include "../components/ir_sensor.h"
#include "../components/light_sensor.h"
#include "../logic/movement.h"

// #define VERBOSE

// interval in microseconds
#define INTERVAL 20000

extern volatile int countdown;
extern volatile int trig;

extern Compass compass;
extern IRSensor *ir_sensors[4];
extern LightSensor *light_sensors[4];

void updateDriving();
void updateAll();

void initTimer();

#endif
