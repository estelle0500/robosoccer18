#include <compass_tests.h>

void testCompass(Compass& compass){
  Serial.println(compass.readAngle());
}
