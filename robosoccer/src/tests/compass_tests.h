#ifndef COMPASS_TESTS_H
#define COMPASS_TESTS_H
#ifndef COMPASS_H
#include "../components/compass.h"
#endif
/*
Purpose: Test if compass is working
Expected Action: Value of angle will be printed to Serial
Troubleshooting: Check wire connection with compass
*/
void testCompass(Compass& compass);
#endif
