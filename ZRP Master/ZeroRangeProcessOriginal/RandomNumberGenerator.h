#pragma once
#include <gsl/gsl_rng.h>
#include <time.h>
#include <thread>
#include <functional>


class RandomNumberGenerator {

private:
	gsl_rng* r;

public:
	RandomNumberGenerator(unsigned long int seed = time(nullptr));
	~RandomNumberGenerator();

	auto NextUniformRealisation() -> double;
	auto NextIntegerRealisation(int maxExcluded) -> int;
	auto NextIntegerRealisation(int minIncluded, int maxExcluded) -> int;
};
