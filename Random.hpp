#pragma once


#include <random>


template<typename T>
class Random
{
	template <typename U>
	static auto dist() -> typename std::enable_if<std::is_integral<U>::value, std::uniform_int_distribution<U>>::type;

	template <typename U>
	static auto dist() -> typename std::enable_if<std::is_floating_point<U>::value, std::uniform_real_distribution<U>>::type;


public:
	Random();

	auto operator()(T max);


private:
	std::mt19937			mRandomEngine;
};

#include "Random.inl"
