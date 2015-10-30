#include "Random.hpp"


Random::Random(int max)
	: mRandomEngine(std::random_device{}())
	, mUniformDistribution(0, max)
{
}

int Random::operator()() const
{
	return mUniformDistribution(mRandomEngine);
}