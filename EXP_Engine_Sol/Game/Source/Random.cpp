#include "Random.h"
#include <random>
#include "../External/PCG/pcg_random.hpp"
#include "../External/PCG/pcg_extras.hpp"

Random::Random(uint num) : num(num) {}

uint Random::GenerateRandomIntRange(int first, int last)
{
	pcg_extras::seed_seq_from<std::random_device> seed;
	pcg32 rng(seed);

	std::uniform_int_distribution<uint> uniform_dist1(first, last);
	int num = num = uniform_dist1(rng);

	return num;;
}

uint Random::GenerateRandomInt()
{
	pcg_extras::seed_seq_from<std::random_device> seed;
	pcg32 rng(seed);

	std::uniform_int_distribution<uint> uniform_dist1(0, 429496);
	int num = num = uniform_dist1(rng);

	return num;
}