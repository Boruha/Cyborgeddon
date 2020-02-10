#include <cmp/Transformable.hpp>
#include <iostream>
#include <glm/gtx/string_cast.hpp>

std::ostream& operator<<(std::ostream& os, const Transformable& trans) {
	trans.print(os, "Transformable")
	<< "\n\tPosition: " << glm::to_string(trans.position)
	<< "\n\tRotation: "	<< glm::to_string(trans.rotation);

	return os;
}
