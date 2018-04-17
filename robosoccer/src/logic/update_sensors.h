#ifndef UPDATE_SENSORS_H
#define UPDATE_SENSORS_H

#include "Arduino.h"
#include "../components/compass.h"
#include "../components/ir_sensor.h"
#include "../components/ultrasound_sensor.h"
#include "../components/light_sensor.h"
#include "../external_libs/Adafruit_ADS1X15/Adafruit_ADS1015.h"
#include "../logic/movement.h"

// #define VERBOSE

// interval in microseconds
#define INTERVAL 20000

extern Adafruit_ADS1115 ads;
extern Compass compass;
extern IRSensor *ir_sensors[4];
extern UltrasoundSensor *ultra_sensors[4];
extern LightSensor *light_sensors[4];

extern volatile bool blocked_dir[4];
const int deg_dirs[4] = {180, 90, 0, -90};
const int deg_dirs_cw[4] = {0, 90, 180, -90};

void updateDriving();
void updateAll();

#endif
