#include <cmp/RigidStaticAABB.hpp>
#include <iostream>
#include <glm/gtx/string_cast.hpp>

std::ostream &operator<<(std::ostream &os, const RigidStaticAABB& cmp) {
	cmp.print(os, "RigidStaticAABB")
			<< "\n\tMin: "							<< glm::to_string(cmp.min)
			<< "\n\tMax: "							<< glm::to_string(cmp.max);

	return os;
}
