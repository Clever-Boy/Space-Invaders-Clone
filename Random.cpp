#include "Random.hpp"


Random::Random()
	: mRandomEngine(std::random_device{}())
{
}

int Random::operator()(int max) const
{
	return (*this)(0, max);
}

int Random::operator()(int min, int max) const
{
	std::uniform_int_distribution<> uniformDistribution(min, max);
	return uniformDistribution(mRandomEngine);
}