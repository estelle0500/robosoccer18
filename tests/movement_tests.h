#ifndef MOVEMENT_TESTS_H

/* 
Purpose: Test if motors are working and turning code is working
Expected Actions: Robot should turn to the target angle
Troubleshooting: Check that motors are working (testMotors()) and check compass (testCompass())
*/
void testTurn(float target=90);

/* Please do not use this. */
void testPattern();

/* Similar to testAlign() but the Robot will only align to face North then stay there */
void testAlignStatic();

/*
Purpose: Test if compass and motors are working
Expected Actions: Robot will align to face North then South
Troubleshooting: Check that motors are working (testMotors()) and check the compass output (testCompass())
*/
void testAlign();

/*
Purpose: Circles look cool
Expected Actions: Robot will move in a circle
Troubleshooting: It doesn't really matter if this doesn't work
*/
void testCircle();

/* 
Purpose: Test if motors are working
Expected Actions: Each motor in the robot should rotate clockwise then counterclockwise 
Troubleshooting: Check wire connections with the motors
*/
void testMotors();

/*
Purpose: ??
Expected Actions: Robot will spin quickly in the clockwise direction
Troubleshooting: Check wire connections with the motors
*/
void testRun();

#endif
