#include <sys/VelocitySystem.hpp>

void VelocitySystem::update(const std::unique_ptr<GameContext> &context, const float deltaTime) const {
	for (auto& cmp : std::get<vector<Velocity>>(context->getComponents(VELOCITY_TYPE)))
		if (cmp.getEntityType() != UNDEFINED)
			if (cmp.currentSpeed != cmp.defaultSpeed)
				cmp.currentSpeed < cmp.defaultSpeed ? accelerate(cmp) : decelerate(cmp);
}

void VelocitySystem::accelerate(Velocity& vel) const {
	if ((vel.currentSpeed += vel.acceleration) > vel.defaultSpeed)
		vel.currentSpeed = vel.defaultSpeed;
}

void VelocitySystem::decelerate(Velocity& vel) const {
	if ((vel.currentSpeed -= vel.acceleration) < vel.defaultSpeed)
		vel.currentSpeed = vel.defaultSpeed;
}