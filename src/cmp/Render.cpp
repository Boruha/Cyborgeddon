#include <cmp/Render.hpp>

#include <ostream>
#include <glm/gtx/string_cast.hpp>

std::ostream& operator<<(std::ostream& os, const Render& ren) {
	ren.print(os, ren.getName())
			<< "\n\tINode: "    << ren.node
			<< "\n\tPosition: " << glm::to_string(*ren.position)
			<< "\n\tRotation: "	<< glm::to_string(*ren.rotation)
			<< "\n\tScale: "    << glm::to_string(*ren.scale);

	return os;
}