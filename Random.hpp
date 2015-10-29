#pragma once


#include <random>

class Random
{
public:
							Random();

	int						operator()(int max) const;
	int						operator()(int min, int max) const;


private:
	mutable std::mt19937	mRandomEngine;
};
