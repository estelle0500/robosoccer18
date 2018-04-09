#pragma once

#include <stdlib.h>
#include <math.h>
#ifndef COMPILE_FOR_ARDUINO
#include <iostream>
#include <random>
#include <time.h>
#endif
#ifdef COMPILE_FOR_ARDUINO
#include <Arduino.h>
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
	double RNG();

	/*
	Generates a pair of gaussians with standard deviation = 1
	*/
	void gaussian(double &R1, double &R2);

	/*
	Generates a pair of gaussians with specified standard deviation
	*/
	void gaussian(double &R1, double &R2, double standardDeviation);
};