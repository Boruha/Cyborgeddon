#include <sys/VelocitySystem.hpp>

void VelocitySystem::update(const std::unique_ptr<GameContext> &context, const float deltaTime) const {
	for (auto& velocity : std::get<vector<Velocity>>(context->getComponents(VELOCITY_TYPE)))
		if (velocity)
			if (velocity.currentSpeed != velocity.defaultSpeed)
				Sun::less_e(velocity.currentSpeed, velocity.defaultSpeed) ? accelerate(velocity, deltaTime) : decelerate(velocity, deltaTime);
}

void VelocitySystem::accelerate(Velocity& vel, const float deltaTime) const {
	if (Sun::greater_e((vel.currentSpeed += vel.acceleration * deltaTime), vel.defaultSpeed))
		vel.currentSpeed = vel.defaultSpeed;
}

void VelocitySystem::decelerate(Velocity& vel, const float deltaTime) const {
	if (Sun::less_e((vel.currentSpeed -= vel.acceleration * deltaTime), vel.defaultSpeed))
		vel.currentSpeed = vel.defaultSpeed;
}