#include <cmp/Transformable.hpp>
#include <ostream>
#include <glm/gtx/string_cast.hpp>

std::ostream& operator<<(std::ostream& os, const Transformable& trans) {
	trans.print(os, trans.getName())
	<< "\n\tPosition: " << glm::to_string(trans.position)
	<< "\n\tRotation: "	<< glm::to_string(trans.rotation)
	<< "\n\tScale: "	<< glm::to_string(trans.scale);

	return os;
}
