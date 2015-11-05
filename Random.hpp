#pragma once


#include <random>


template<typename T, class R = std::mt19937>
class Random
{
	static_assert(std::is_integral<T>::value || std::is_floating_point<T>::value, "!");

	using UniformInt	= std::uniform_int_distribution<T>;
	using UniformReal	= std::uniform_real_distribution<T>;
	using DistType		= std::conditional_t<std::is_integral<T>::value, UniformInt, UniformReal>;


public:
							Random();

	T						operator()(T max) const;

	T						operator()(T min, T max) const;


private:
							template<class U = R, std::size_t N = U::state_size>
	static auto				ProperlySeededRandomEngine() -> typename std::enable_if_t<!!N, U>;


private:
	mutable R				mRandomEngine;
};


using RandomInt		= Random<int>;
using RandomFloat	= Random<float>;
using RandomDouble	= Random<double>;


#include "Random.inl"