#ifndef FASTRNG_H
#define FASTRNG_H
#define COMPILE_FOR_ARDUINO

#include <stdlib.h>
#include <math.h>
#include "SpeedTrig.h"
#ifndef COMPILE_FOR_ARDUINO
//#include <iostream>
//#include <random>
//#include <time.h>
#endif
#ifdef COMPILE_FOR_ARDUINO
//#include <random>
#endif


using namespace std;

// or actually not anything faster
class fastRNG
{
public :
	/*
	Initializer. Initialize the randomseed
	*/
	fastRNG();

	/*
	Generates a random number from 0 to 1
	*/
	float RNG();

	/*
	Generates a pair of gaussians with standard deviation = 1
	*/
	void gaussian(float &R1, float &R2);

	/*
	Generates a pair of gaussians with specified standard deviation
	*/
	void gaussian(float &R1, float &R2, float standardDeviation);
};

#endif // !FASTRNG_H
