#pragma once


#include <random>
#include <type_traits>


template<typename T>
class Random
{
	static_assert(std::is_integral<T>::value || std::is_floating_point<T>::value, "!");

	using DistType = std::conditional_t<std::is_integral<T>::value, std::uniform_int_distribution<T>, std::uniform_real_distribution<T>>;


public:
							Random();

	T						operator()(T max) const;
	T						operator()(T min, T max) const;


private:
	mutable std::mt19937	mRandomEngine;
};


using RandomInt		= Random<int>;
using RandomFloat	= Random<float>;
using RandomDouble	= Random<double>;


#include "Random.inl"