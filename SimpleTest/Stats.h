#ifndef STATS_H
#define STATS_H

#ifdef COMPILE_FOR_ARDUINO
#include <math.h>
#endif // COMPILE_FOR_ARDUINO
#ifndef COMPILE_FOR_ARDUINO
#include <cmath>
#endif // COMPILE_FOR_ARDUINO

// somewhat optimized with the use of look-up table, but not yet tested

using namespace std;

class Stats
{
public:
	// the factor for 1/sqrt(2Pi)
	//const float gaussianFactor = 1.0 / sqrt(2 * 3.1415926);

	/**
	Compute the gaussian function value with a given standard deviation
	*/
	float gaussian(float x, float stdDeviation)
	{
		return gaussian(x / stdDeviation) / stdDeviation;
	}

	/**
	Assume that the standard deviation is unity
	Compute the gaussian function value
	*/
	float gaussian(float x)
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

	float exponent(float x)
	{
		//return exp(x);
		if (x > 0)
			return 0;  //Prevent the function from erroring out
		x = -x;
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
	float expDist(float x, float xMax, float lamdaShort)
	{
		if (x <= xMax)
			return (float)(1.0 / (1 - exponent(-lamdaShort*xMax)))*lamdaShort*exponent(-lamdaShort*x);
		else
			return 0;
	}

private:
	// standard gaussion, from 0 to 4, with 0.05 step size, so 81 elements
	float gaussianTable[81] = { (float) 0.398942,(float) 0.398444,(float) 0.396953,(float) 0.394479,(float) 0.391043,(float) 0.386668,(float) 0.381388,(float) 0.37524,(float) 0.36827,(float) 0.360527,(float) 0.352065,(float) 0.342944,(float) 0.333225,(float) 0.322972,(float) 0.312254,(float) 0.301137,(float) 0.289692,(float) 0.277985,(float) 0.266085,(float) 0.254059,(float) 0.241971,(float) 0.229882,(float) 0.217852,(float) 0.205936,(float) 0.194186,(float) 0.182649,(float) 0.171369,(float) 0.160383,(float) 0.149727,(float) 0.139431,(float) 0.129518,(float) 0.120009,(float) 0.110921,(float) 0.102265,(float) 0.0940491,(float) 0.0862773,(float) 0.0789502,(float) 0.0720649,(float) 0.0656158,(float) 0.0595947,(float) 0.053991,(float) 0.048792,(float) 0.0439836,(float) 0.03955,(float) 0.0354746,(float) 0.0317397,(float) 0.028327,(float) 0.0252182,(float) 0.0223945,(float) 0.0198374,(float) 0.0175283,(float) 0.0154493,(float) 0.013583,(float) 0.0119122,(float) 0.0104209,(float) 0.00909356,(float) 0.00791545,(float) 0.00687277,(float) 0.00595253,(float) 0.00514264,(float) 0.00443185,(float) 0.00380976,(float) 0.00326682,(float) 0.00279426,(float) 0.00238409,(float) 0.00202905,(float) 0.00172257,(float) 0.00145873,(float) 0.00123222,(float) 0.00103828,(float) 0.000872683,(float) 0.000731664,(float) 0.000611902,(float) 0.000510465,(float) 0.00042478,(float) 0.000352596,(float) 0.000291947,(float) 0.000241127,(float) 0.000198655,(float) 0.000163256,(float) 0.00013383 };
	// exp(-x), from 0 to 5, with 0.05 step size, so 101 elements
	float exponentialTable[101] = { (float) 1.,(float) 0.951229,(float) 0.904837,(float) 0.860708,(float) 0.818731,(float) 0.778801,(float) 0.740818,(float) 0.704688,(float) 0.67032,(float) 0.637628,(float) 0.606531,(float) 0.57695,(float) 0.548812,(float) 0.522046,(float) 0.496585,(float) 0.472367,(float) 0.449329,(float) 0.427415,(float) 0.40657,(float) 0.386741,(float) 0.367879,(float) 0.349938,(float) 0.332871,(float) 0.316637,(float) 0.301194,(float) 0.286505,(float) 0.272532,(float) 0.25924,(float) 0.246597,(float) 0.23457,(float) 0.22313,(float) 0.212248,(float) 0.201897,(float) 0.19205,(float) 0.182684,(float) 0.173774,(float) 0.165299,(float) 0.157237,(float) 0.149569,(float) 0.142274,(float) 0.135335,(float) 0.128735,(float) 0.122456,(float) 0.116484,(float) 0.110803,(float) 0.105399,(float) 0.100259,(float) 0.0953692,(float) 0.090718,(float) 0.0862936,(float) 0.082085,(float) 0.0780817,(float) 0.0742736,(float) 0.0706512,(float) 0.0672055,(float) 0.0639279,(float) 0.0608101,(float) 0.0578443,(float) 0.0550232,(float) 0.0523397,(float) 0.0497871,(float) 0.0473589,(float) 0.0450492,(float) 0.0428521,(float) 0.0407622,(float) 0.0387742,(float) 0.0368832,(float) 0.0350844,(float) 0.0333733,(float) 0.0317456,(float) 0.0301974,(float) 0.0287246,(float) 0.0273237,(float) 0.0259911,(float) 0.0247235,(float) 0.0235177,(float) 0.0223708,(float) 0.0212797,(float) 0.0202419,(float) 0.0192547,(float) 0.0183156,(float) 0.0174224,(float) 0.0165727,(float) 0.0157644,(float) 0.0149956,(float) 0.0142642,(float) 0.0135686,(float) 0.0129068,(float) 0.0122773,(float) 0.0116786,(float) 0.011109,(float) 0.0105672,(float) 0.0100518,(float) 0.0095616,(float) 0.00909528,(float) 0.0086517,(float) 0.00822975,(float) 0.00782838,(float) 0.00744658,(float) 0.00708341,(float) 0.00673795 };
};


//Stats calculator;
//cout << "StandardDeviation[{";
//for (int i = -999; i < 1000; i++)
//	cout << "{" << (float)i / 10 << ",Internal`StringTofloat[\"" << calculator.gaussian((float)i / 10, 10) << "\"]},";
//cout << "{0,0}}]";
//char c;
//cin >> c;
#endif // !STATS_H