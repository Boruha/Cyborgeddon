#include <cmp/Velocity.hpp>

std::ostream& operator<<(std::ostream& os, const Velocity& vel) {
	vel.print(os, "Velocity")
	<< "\n\tDirection: "	<< vel.direction
	<< "\n\tCurrentSpeed: " << vel.currentSpeed
	<< "\n\tDefaultSpeed: " << vel.defaultSpeed
	<< "\n\tAcceleration: " << vel.acceleration;

	return os;
}