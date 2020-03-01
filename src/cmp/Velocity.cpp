#include <cmp/Velocity.hpp>
#include <ostream>
#include <glm/gtx/string_cast.hpp>

std::ostream& operator<<(std::ostream& os, const Velocity& vel) {
	vel.print(os, vel.getName())
	<< "\n\tDirection: "	<< glm::to_string(vel.direction)
	<< "\n\tCurrentSpeed: " << vel.currentSpeed
	<< "\n\tDefaultSpeed: " << vel.defaultSpeed
	<< "\n\tAcceleration: " << vel.acceleration;

	return os;
}