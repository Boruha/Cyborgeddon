#include <cmp/CircleBounding.hpp>
#include <glm/gtx/string_cast.hpp>
#include <iostream>

std::ostream& operator<<(std::ostream& os, const CircleBounding& bounding) {
	bounding.print(os, CircleBounding::getName())
			<< "\n\tMin: "      << glm::to_string(bounding.min)
			<< "\n\tMax: "      << glm::to_string(bounding.max)
			<< "\n\tCenter: "   << glm::to_string(bounding.center)
			<< "\n\tRadius: "   << bounding.radius
			<< "\n\tVelocity: " << glm::to_string(*bounding.velocity);

	return os;
}