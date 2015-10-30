#pragma once


#include <random>


class Random
{
	using DistType = std::uniform_int_distribution<>;


public:
	explicit				Random(int max);

	int						operator()() const;


private:
	mutable std::mt19937	mRandomEngine;
	DistType				mUniformDistribution;
};
