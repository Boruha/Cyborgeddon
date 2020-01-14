#include <cmp/Physics.hpp>

std::ostream& operator<<(std::ostream& os, const Physics& phy) {
	phy.print(os, "Physics")
	<< "\n\tPosition: " << phy.position
	<< "\n\tRotation: "	<< phy.rotation
	<< "\n\tVelocity: " << phy.velocity;

	return os;
}
