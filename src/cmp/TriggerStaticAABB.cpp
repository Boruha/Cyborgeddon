#include <cmp/TriggerStaticAABB.hpp>
#include <iostream>
#include <glm/gtx/string_cast.hpp>

std::ostream &operator<<(std::ostream &os, const TriggerStaticAABB &cmp) {
	cmp.print(os, "TriggerStaticAABB")
	<< "\n\tPassable: " << std::boolalpha 	<< cmp.passable
	<< "\n\tMin: "							<< glm::to_string(cmp.min)
	<< "\n\tMax: "							<< glm::to_string(cmp.max);
}
