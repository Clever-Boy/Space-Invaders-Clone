#include <cmath>

template <typename Resource>
void centerOrigin(Resource& resource)
{
	auto bounds(resource.getLocalBounds());
	resource.setOrigin(std::floor(bounds.left + bounds.width / 2.f), std::floor(bounds.top + bounds.height / 2.f));
}