#include <sys/CooldownSystem.hpp>
#include <Engine/util/Math.hpp>

void CooldownSystem::update(const std::unique_ptr<GameContext>& context, const float deltaTime) {
	for (auto& data : context->getComponents().get<CharacterData>()) {
		if (less_e((data.currentAttackingCooldown -= deltaTime), 0))
			data.currentAttackingCooldown = 0.f;
		if (less_e((data.currentSwitchingCooldown -= deltaTime), 0))
			data.currentSwitchingCooldown = 0.f;
		if (less_e(data.currentDashingCooldown -= deltaTime, 0))
		    data.currentDashingCooldown = 0.f;
	}
}
