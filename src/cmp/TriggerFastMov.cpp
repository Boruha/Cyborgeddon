#include <cmp/TriggerFastMov.hpp>
#include <ostream>
#include <glm/gtx/string_cast.hpp>

std::ostream& operator<<(std::ostream& os, const TriggerFastMov& cmp) {
	cmp.print(os, cmp.getName())
			<< "\n\tPosition: " << glm::to_string(cmp.position)
			<< "\n\tVelocity: " << glm::to_string(cmp.velocity);

	return os;
}