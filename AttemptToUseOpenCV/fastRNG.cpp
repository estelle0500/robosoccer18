#pragma once
#include "fastRNG.h"


using namespace std;


#ifndef COMPILE_FOR_ARDUINO
fastRNG::fastRNG()
{
	srand(time(NULL));
}

double fastRNG::RNG()
{
	double ans = (double)rand() / (double)RAND_MAX;
	return ans;
}
#endif


#ifdef COMPILE_FOR_ARDUINO

fastRNG::fastRNG()
{
	randomseed();
}

double fastRNG::RNG()
{
	double ans = double(random(32768)) / 32768.0;
	return ans;
}

#endif

void fastRNG::gaussian(double &R1, double &R2)
{
	double U1 = RNG();
	double U2 = RNG();
	double sigma = sqrt(-2 * log(U1));
	R1 = sigma*cos(6.18*U2);
	R2 = sigma*sin(6.18*U2);
}

void fastRNG::gaussian(double &R1, double &R2, double standardDeviation)
{
	gaussian(R1, R2);
	R1 = R1 * standardDeviation;
	R2 = R2 * standardDeviation;
}