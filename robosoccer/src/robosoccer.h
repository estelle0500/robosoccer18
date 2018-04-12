#ifndef ROBOSOCCER_H
#define ROBOSOCCER_H

#include "Arduino.h"
// #include "external_libs/Adafruit-GFX/Adafruit_GFX.h"
// #include "external_libs/Adafruit_SSD1306/Adafruit_SSD1306.h"
#include "external_libs/i2c_t3/i2c_t3.h"
// #include "external_libs/Adafruit_VL53L0X/Adafruit_VL53L0X.h"
#include "external_libs/VL53L0X/VL53L0X.h"
#include "external_libs/SpeedTrig/SpeedTrig.h"

#include "components/compass.h"
// #include "components/lcd.h"
#include "components/motor.h"
#include "components/ir_sensor.h"
#include "components/light_sensor.h"

#include "logic/movement.h"
#include "logic/update_sensors.h"

#include "tests/movement_tests.h"
#include "tests/compass_tests.h"

#endif
