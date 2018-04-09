#pragma once
#include "fastRNG.h"
#include <stdlib.h>
#include <iostream>
#include <random>

using namespace std;

void fastRNG::generateNewList()
{
	for (int i = 0; i < 10000; i++)
	{
		RNGlist[i] = (double)rand() / (double)RAND_MAX - 0.5;
	}
	currentPos = 0;
}

fastRNG::fastRNG()
{
	generateNewList();
}

double fastRNG::RNG()
{
	double ans = RNGlist[currentPos];
	currentPos = (currentPos + 1) % 10000;
	return ans;
}

double fastRNG::gaussian()
{
	double sum = 0;
	for (int i = 0; i < 12; i++)
		sum  = sum + RNG();
	return sum ;
}

double fastRNG::gaussian(double variance)
{
	return gaussian()*variance;
}