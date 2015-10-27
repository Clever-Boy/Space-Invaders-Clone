template<typename T>
Random<T>::Random()
	: mRandomEngine(std::random_device{}())
{
}

template<typename T>
T Random<T>::operator()(T max)
{
	return (*this)(0, max);
}

template<typename T>
T Random<T>::operator()(T min, T max)
{
	dist_type uniformDistribution(min, max);
	return uniformDistribution(mRandomEngine);
}