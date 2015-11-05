#include <array>


template<typename T, class R>
Random<T, R>::Random()
	: mRandomEngine(ProperlySeededRandomEngine())
{
}

template<typename T, class R>
T Random<T, R>::operator()(T max) const
{
	return (*this)(0, max);
}

template<typename T, class R>
T Random<T, R>::operator()(T min, T max) const
{
	DistType mUniformDistribution(min, max);
	return mUniformDistribution(mRandomEngine);
}

template<typename T, class R>
template<class U, std::size_t N>
auto Random<T, R>::ProperlySeededRandomEngine() -> typename std::enable_if_t<!!N, U>
{
	std::array<typename U::result_type, N> random_data;
	std::random_device source;
	std::generate(random_data.begin(), random_data.end(), std::ref(source));
	std::seed_seq seeds(random_data.begin(), random_data.end());
	U seeded_engine(seeds);
	return seeded_engine;
}