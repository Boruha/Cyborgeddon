#include <cmp/TriangleOBB.hpp>
#include <glm/gtx/string_cast.hpp>
#include <ostream>

std::ostream& operator<<(std::ostream& os, const TriangleOBB & obb) {
	obb.print(os, TriangleOBB::getName())
			<< "\n\tMin: " << glm::to_string(obb.min)
			<< "\n\tMax: " << glm::to_string(obb.max)
			<< "\n\tVertice 0: (" << glm::to_string(obb.vertices[0])
			<< "\n\tVertice 1: (" << glm::to_string(obb.vertices[1])
			<< "\n\tVertice 2: (" << glm::to_string(obb.vertices[2]);

	return os;
}