#include <cmp/Render.hpp>

#include <ostream>
#include <glm/gtx/string_cast.hpp>

std::ostream& operator<<(std::ostream& os, const Render& ren) {
	ren.print(os, Render::getName())
			<< "\n\tINode: "    << ren.node
			<< "\n\tPosition: " << glm::to_string(*ren.pos)
			<< "\n\tRotation: "	<< glm::to_string(*ren.rot)
			<< "\n\tScale: "    << glm::to_string(*ren.sca);

	return os;
}