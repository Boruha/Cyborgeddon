#include <sys/CooldownSystem.hpp>
#include <Engine/util/Math.hpp>

void CooldownSystem::update(const Context& context, const float deltaTime) {
	auto& storage = context->getComponents();
	
	for (auto& data : storage.getComponents<CharacterData>()) 
	{
		if (less_e((data.currentAttackingCooldown -= deltaTime), 0))
			data.currentAttackingCooldown = 0.f;
		if (less_e((data.currentSwitchingCooldown -= deltaTime), 0))
			data.currentSwitchingCooldown = 0.f;
		if (less_e(data.currentDashingCooldown -= deltaTime, 0))
		    data.currentDashingCooldown = 0.f;
		
	}
	for(auto& jump : storage.getComponents<Jump>())
	{
		if (less_e(jump.currentJumpCooldown -= deltaTime, 0))
		    jump.currentJumpCooldown = 0.f;
	}
}
