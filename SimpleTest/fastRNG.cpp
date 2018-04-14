#include "fastRNG.h"


using namespace std;


#ifndef COMPILE_FOR_ARDUINO
fastRNG::fastRNG()
{
	srand(time(NULL));
}

float fastRNG::RNG()
{
	float ans = (float)rand() / (float)RAND_MAX;
	return ans;
}
#endif


#ifdef COMPILE_FOR_ARDUINO

fastRNG::fastRNG()
{
	srand(10);
}

float fastRNG::RNG()
{
	float ans = float(rand()%32768) / 32768.0;
	return ans;
}

#endif

void fastRNG::gaussian(float &R1, float &R2)
{
	float U1 = RNG();
	float U2 = RNG();
	float sigma = sqrt(-2 * log(U1));
	R1 = (float) sigma*SpeedTrig.cos(6.18*U2);
	R2 = sigma*SpeedTrig.sin(6.18*U2);
}

void fastRNG::gaussian(float &R1, float &R2, float standardDeviation)
{
	gaussian(R1, R2);
	R1 = R1 * standardDeviation;
	R2 = R2 * standardDeviation;
}

