#include "RandomNumberGenerator.h"

RandomNumberGenerator::RandomNumberGenerator(unsigned long int seed)
{
	gsl_rng_env_setup();
	r = gsl_rng_alloc(gsl_rng_default);
	std::thread::id thread_id = std::this_thread::get_id();
	auto hashed_id = std::hash<std::thread::id>{}(thread_id);
	gsl_rng_set(r, seed ^ hashed_id);
}

auto RandomNumberGenerator::NextUniformRealisation() -> double
{
	return gsl_rng_uniform(r);
}

auto RandomNumberGenerator::NextIntegerRealisation(int maxExcluded) -> int
{
	return NextIntegerRealisation(0, maxExcluded);
}

auto RandomNumberGenerator::NextIntegerRealisation(int minIncluded, int maxExcluded) -> int
{
	return minIncluded + gsl_rng_uniform_int(r, maxExcluded - minIncluded);
}

RandomNumberGenerator::~RandomNumberGenerator() {
	gsl_rng_free(r);
}
