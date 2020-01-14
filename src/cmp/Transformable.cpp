#include <cmp/Transformable.hpp>

std::ostream& operator<<(std::ostream& os, const Transformable& trans) {
	trans.print(os, "Transformable")
	<< "\n\tPosition: " << trans.position
	<< "\n\tRotation: "	<< trans.rotation;

	return os;
}
