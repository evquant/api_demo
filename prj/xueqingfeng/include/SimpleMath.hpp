#ifndef SIMPLE_MATH_H_
#define SIMPLE_MATH_H_

#include "Array.h"

inline double Summation(NumericSeries prices, int period)
{
	double sum = 0.0;
	for (int i = 0; i < period; ++i)
	{
		sum += prices[i]; 
	}
	return sum;
}

inline double Average(NumericSeries prices, int period)
{
	return Summation(prices, period) / min(period, prices.Size());
}

inline double StandardDev(NumericSeries avgs, NumericSeries prices, int period)
{
	double dev = 0.0;
	for (int i = 0; i < min(period, prices.Size()); ++i) {
		dev += pow(avgs[i] - prices[i], 2);
	}
	return sqrt(dev / min(period, prices.Size()));
}

#endif