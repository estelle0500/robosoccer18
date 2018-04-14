#ifndef ROBOSOCCER_H
#define ROBOSOCCER_H

#include "Arduino.h"
// #include "external_libs/Adafruit-GFX/Adafruit_GFX.h"
// #include "external_libs/Adafruit_SSD1306/Adafruit_SSD1306.h"
#include "external_libs/i2c_t3/i2c_t3.h"
// #include "external_libs/Adafruit_VL53L0X/Adafruit_VL53L0X.h"
#include "external_libs/VL53L0X/VL53L0X.h"
#include "external_libs/SpeedTrig/SpeedTrig.h"
#include "external_libs/Adafruit_ADS1X15/Adafruit_ADS1015.h"

#include "components/compass.h"
// #include "components/lcd.h"
#include "components/motor.h"
#include "components/ir_sensor.h"
#include "components/light_sensor.h"
#include "components/ultrasound_sensor.h"

#include "logic/movement.h"
#include "logic/update_sensors.h"

#include "tests/movement_tests.h"
#include "tests/compass_tests.h"

#define DCHANNEL1 18, 47, 20, 21
#define DCHANNEL2 19, 47, 22, 23
#define DCHANNEL3 28, 47, 30, 29
#define DCHANNEL4 7, 47, 6, 5
#define DCHANNEL5 10, 47, 9, 8
#define DCHANNEL6 4, 47, 3, 2

#define MULTIPLEXER_ADDRESS 0x70

extern LightSensor *light_sensors[4];

void setup_components();
void waitKey();

#endif
