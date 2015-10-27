template<typename T>
Random<T>::Random()
	: mRandomEngine(std::random_device{}())
{
}

template<typename T>
T Random<T>::operator()(T max) const
{
	return (*this)(0, max);
}

template<typename T>
T Random<T>::operator()(T min, T max) const
{
	DistType uniformDistribution(min, max);
	return uniformDistribution(mRandomEngine);
}