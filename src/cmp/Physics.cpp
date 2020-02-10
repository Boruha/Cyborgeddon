#include <cmp/Physics.hpp>
#include <iostream>
#include <glm/gtx/string_cast.hpp>

std::ostream& operator<<(std::ostream& os, const Physics& phy) {
	phy.print(os, "Physics")
	<< "\n\tPosition: " << glm::to_string(phy.position)
	<< "\n\tRotation: "	<< glm::to_string(phy.rotation)
	<< "\n\tVelocity: " << glm::to_string(phy.velocity);

	return os;
}
