template<typename T>
Random<T>::Random()
	: mRandomEngine(std::random_device()())
{
}

template<typename T>
auto Random<T>::operator()(T max)
{
	decltype(dist<T>()) uniformDistribution(0, max - 1);
	return uniformDistribution(mRandomEngine);
}

template<typename T>
auto Random<T>::operator()(T min, T max)
{
	decltype(dist<T>()) uniformDistribution(min, max);
	return uniformDistribution(mRandomEngine);
}