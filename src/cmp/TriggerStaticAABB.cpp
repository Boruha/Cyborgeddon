#include <cmp/TriggerStaticAABB.hpp>
#include <ostream>
#include <glm/gtx/string_cast.hpp>

std::ostream &operator<<(std::ostream &os, const TriggerStaticAABB &cmp) {
	cmp.print(os, cmp.getName())
	<< "\n\tPassable: " << std::boolalpha 	<< cmp.passable
	<< "\n\tMin: "							<< glm::to_string(cmp.min)
	<< "\n\tMax: "							<< glm::to_string(cmp.max);

	return os;
}
