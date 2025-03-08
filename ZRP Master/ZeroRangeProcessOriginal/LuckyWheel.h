#pragma once
#include "RandomNumberGenerator.h"
#include <algorithm>


class LuckyWheel {

public:
	//length 2 * SegmentCount
	double* LookupTableTree; // todo private


private:
	int SegmentCount;
	RandomNumberGenerator* _RandomNumberGenerator;

	auto inline UpdateLookupTableAtSite(int* lattice, double* rateLookupTable, int site)	-> void;

public:
	LuckyWheel(int segmentCount, RandomNumberGenerator *randomNumberGenerator);
	~LuckyWheel();
	auto Initialize(int* lattice, double* rateLookupTable)				-> void;
	auto Update(
		int* lattice,
		double* rateLookupTable,
		int rateDecreaseSite, 
		int rateIncreaseSite)							-> void;
	auto Spin() 									-> const int;


	const double constexpr RateSum() {
		return LookupTableTree[1];
	}
};