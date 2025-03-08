#include "LuckyWheel.h"

auto inline LuckyWheel::UpdateLookupTableAtSite(int* lattice, double* rateLookupTable, int site) -> void
{
	site += SegmentCount;
	double temp = LookupTableTree[site];
	auto siteIndex = site - SegmentCount;
	auto particleCount = lattice[siteIndex];
	LookupTableTree[site] = rateLookupTable[particleCount];
	double delta = LookupTableTree[site] - temp;

	while (site > 1)
	{
		site >>= 1;
		temp = LookupTableTree[site];
		LookupTableTree[site] = LookupTableTree[site] + delta; // std::max(0.0, LookupTableTree[site] + delta);
		delta = LookupTableTree[site] - temp;
	}
}

LuckyWheel::LuckyWheel(int segmentCount, RandomNumberGenerator *randomNumberGenerator)
{
	_RandomNumberGenerator = randomNumberGenerator;
	SegmentCount = segmentCount;
	LookupTableTree = new double[2 * SegmentCount];
}

LuckyWheel::~LuckyWheel()
{
	delete LookupTableTree;
}

auto LuckyWheel::Initialize(int* lattice, double* rateLookupTable) -> void
{
	for (int i = 0; i < SegmentCount; i++)
		LookupTableTree[SegmentCount + i] = rateLookupTable[lattice[i]];

	for (int i = SegmentCount - 1; i > 0; i--)
		LookupTableTree[i] = LookupTableTree[(i << 1)] + LookupTableTree[(i << 1) + 1];
}

auto LuckyWheel::Update(int* lattice, double* rateLookupTable, int rateDecreaseSite, int rateIncreaseSite) -> void
{
	UpdateLookupTableAtSite(lattice, rateLookupTable, rateDecreaseSite);
	UpdateLookupTableAtSite(lattice, rateLookupTable, rateIncreaseSite);
}

auto LuckyWheel::Spin() -> const int
{
	double rnd = _RandomNumberGenerator->NextUniformRealisation();
	double randomNumber
		= LookupTableTree[1] * rnd;
	int i = 1;

	while (i < SegmentCount)
	{
		i <<= 1;
		if (LookupTableTree[i] < randomNumber)
		{
			randomNumber -= LookupTableTree[i];
			i++;
		}
	}
	return i - SegmentCount;
}
