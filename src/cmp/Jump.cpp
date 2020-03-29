#include <cmp/Jump.hpp>
#include<ostream>


Jump::Jump(const EntityType type, const EntityID e_ID, const float cd, const float timer) :
            Component(type, e_ID), jumpCooldown(cd), jumpTimer(timer) {}


std::ostream& operator<<(std::ostream& os, const Jump& jump)
{
    jump.print(os, jump.getName());

	os	<< "\n\tStatus: " 	<< std::boolalpha 	    << jump.jumping
		<< "\n\tCD:     "							<< jump.jumpCooldown
		<< "\n\tCCD:    "							<< jump.currentJumpCooldown
		<< "\n\tTimer:  "							<< jump.jumpTimer

		<< "\n\tTarget Location X: "							<< jump.jumpTargetLocation.x
		<< "\n\tTarget Location Y: "							<< jump.jumpTargetLocation.y
		<< "\n\tTarget Location Z: "							<< jump.jumpTargetLocation.z;

	return os;
}
