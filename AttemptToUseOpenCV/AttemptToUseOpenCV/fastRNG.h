#pragma once
#include <stdlib.h>

using namespace std;

class fastRNG
{
public :
	fastRNG();
	void generateNewList();
	double RNG();
	double gaussian();
	double gaussian(double variance);
private :
	double RNGlist[10000];
	int currentPos;
};