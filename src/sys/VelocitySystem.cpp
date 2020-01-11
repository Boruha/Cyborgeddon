#include <sys/VelocitySystem.hpp>

void VelocitySystem::update(const std::unique_ptr<GameContext> &context, const float deltaTime) const {
	for (auto& cmp : context->getVelocityComponents())
		if (cmp.getEntityType() != UNDEFINED)
			if (cmp.speed != cmp.defaultSpeed)
				cmp.speed < cmp.defaultSpeed ? accelerate(cmp) : decelerate(cmp);
}

void VelocitySystem::accelerate(Velocity& vel) const {
	if ((vel.speed += vel.acceleration) > vel.defaultSpeed)
		vel.speed = vel.defaultSpeed;
}

void VelocitySystem::decelerate(Velocity& vel) const {
	if ((vel.speed -= vel.acceleration) < vel.defaultSpeed)
		vel.speed = vel.defaultSpeed;
}