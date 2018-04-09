#pragma once
#include <cmath>

// somewhat optimized with the use of look-up table, but not yet tested

using namespace std;

class Stats
{
public:
	// the factor for 1/sqrt(2Pi)
	//const double gaussianFactor = 1.0 / sqrt(2 * 3.1415926);

	/**
	Compute the gaussian function value with a given standard deviation
	*/
	double gaussian(double x, double stdDeviation)
	{
		return gaussian(x / stdDeviation) / stdDeviation;
	}

	/**
	Assume that the standard deviation is unity
	Compute the gaussian function value
	*/
	double gaussian(double x)
	{
		// using the property that gaussian is an even function to simply evaluation
		if (x < 0)
			x = -x;
		//calculate the index in the look up table
		int ind = (int)(x * 20.0 + 0.5);
		//constain the index. If overbound, just take the smallest probability value in the table.
		if (ind > 80)
			ind = 80;
		return gaussianTable[ind];
		//return gaussianFactor * exp(-x*x / 2.0);
	}

	/**
	Compute exp(-x)
	*/

	double exponent(double x)
	{
		if (x < 0)
			return 0;  //Prevent the function from erroring out
		//calculate the index in the look up table
		int ind = (int)(x * 20.0 + 0.5);
		//constain the index. If overbound, just take the smallest probability value in the table.
		if (ind > 100)
			ind = 100;
		return exponentialTable[ind];
	}

	/**
	Compute the normalized truncated exp distribution used for sensor reading prob
	*/
	double expDist(double x, double xMax, double lamdaShort)
	{
		if (x <= xMax)
			return 1.0 / (1 - exponent(-lamdaShort*xMax))*lamdaShort*exponent(-lamdaShort*x);
		else
			return 0;
	}

private:
	// standard gaussion, from 0 to 4, with 0.05 step size, so 81 elements
	const double gaussianTable[81] = { 0.398942,0.398444,0.396953,0.394479,0.391043,0.386668,0.381388,0.37524,0.36827,0.360527,0.352065,0.342944,0.333225,0.322972,0.312254,0.301137,0.289692,0.277985,0.266085,0.254059,0.241971,0.229882,0.217852,0.205936,0.194186,0.182649,0.171369,0.160383,0.149727,0.139431,0.129518,0.120009,0.110921,0.102265,0.0940491,0.0862773,0.0789502,0.0720649,0.0656158,0.0595947,0.053991,0.048792,0.0439836,0.03955,0.0354746,0.0317397,0.028327,0.0252182,0.0223945,0.0198374,0.0175283,0.0154493,0.013583,0.0119122,0.0104209,0.00909356,0.00791545,0.00687277,0.00595253,0.00514264,0.00443185,0.00380976,0.00326682,0.00279426,0.00238409,0.00202905,0.00172257,0.00145873,0.00123222,0.00103828,0.000872683,0.000731664,0.000611902,0.000510465,0.00042478,0.000352596,0.000291947,0.000241127,0.000198655,0.000163256,0.00013383 };
	// exp(-x), from 0 to 5, with 0.05 step size, so 101 elements
	const double exponentialTable[101] = { 1.,0.951229,0.904837,0.860708,0.818731,0.778801,0.740818,0.704688,0.67032,0.637628,0.606531,0.57695,0.548812,0.522046,0.496585,0.472367,0.449329,0.427415,0.40657,0.386741,0.367879,0.349938,0.332871,0.316637,0.301194,0.286505,0.272532,0.25924,0.246597,0.23457,0.22313,0.212248,0.201897,0.19205,0.182684,0.173774,0.165299,0.157237,0.149569,0.142274,0.135335,0.128735,0.122456,0.116484,0.110803,0.105399,0.100259,0.0953692,0.090718,0.0862936,0.082085,0.0780817,0.0742736,0.0706512,0.0672055,0.0639279,0.0608101,0.0578443,0.0550232,0.0523397,0.0497871,0.0473589,0.0450492,0.0428521,0.0407622,0.0387742,0.0368832,0.0350844,0.0333733,0.0317456,0.0301974,0.0287246,0.0273237,0.0259911,0.0247235,0.0235177,0.0223708,0.0212797,0.0202419,0.0192547,0.0183156,0.0174224,0.0165727,0.0157644,0.0149956,0.0142642,0.0135686,0.0129068,0.0122773,0.0116786,0.011109,0.0105672,0.0100518,0.0095616,0.00909528,0.0086517,0.00822975,0.00782838,0.00744658,0.00708341,0.00673795 };
};


//Stats calculator;
//cout << "StandardDeviation[{";
//for (int i = -999; i < 1000; i++)
//	cout << "{" << (double)i / 10 << ",Internal`StringToDouble[\"" << calculator.gaussian((double)i / 10, 10) << "\"]},";
//cout << "{0,0}}]";
//char c;
//cin >> c;