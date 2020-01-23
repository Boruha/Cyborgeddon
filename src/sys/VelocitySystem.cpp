#include <sys/VelocitySystem.hpp>

void VelocitySystem::update(const std::unique_ptr<GameContext> &context, const float deltaTime) const {
	for (auto& cmp : std::get<vector<Velocity>>(context->getComponents(VELOCITY_TYPE)))
		if (cmp.getEntityType() != UNDEFINED)
			if (cmp.currentSpeed != cmp.defaultSpeed)
				Sun::less_e(cmp.currentSpeed, cmp.defaultSpeed) ? accelerate(cmp, deltaTime) : decelerate(cmp, deltaTime);
}

void VelocitySystem::accelerate(Velocity& vel, const float deltaTime) const {
	if (Sun::greater_e((vel.currentSpeed += vel.acceleration * deltaTime), vel.defaultSpeed))
		vel.currentSpeed = vel.defaultSpeed;
}

void VelocitySystem::decelerate(Velocity& vel, const float deltaTime) const {
	if (Sun::less_e((vel.currentSpeed -= vel.acceleration * deltaTime), vel.defaultSpeed))
		vel.currentSpeed = vel.defaultSpeed;
}