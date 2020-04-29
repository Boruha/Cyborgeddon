#pragma once

#include <cmp/Component.hpp>


struct Jump : Component
{
    explicit Jump(EntityType, EntityID, float, float);

	[[nodiscard]] constexpr static std::string_view getName() {
		return "Jump Data";
	}
    
	friend std::ostream& operator<<(std::ostream&, const Jump&);

	bool		jumping 			{ false };
	float 		jumpCooldown		{   0.f };
	float 		currentJumpCooldown	{   0.f };
	float 		jumpTimer           {   0.f };
	vec3 		jumpTargetLocation  {   0   };
};
