#include <sys/VelocitySystem.hpp>
#include <Engine/util/Math.hpp>

void VelocitySystem::update(const std::unique_ptr<GameContext> &context, const float deltaTime) const {
	for (auto& velocity : std::get<vector<Velocity>>(context->getComponents(VELOCITY_TYPE)))
		if (velocity && velocity.currentSpeed != velocity.defaultSpeed)
			less_e(velocity.currentSpeed, velocity.defaultSpeed) ? accelerate(velocity, deltaTime) : decelerate(velocity, deltaTime);
}

void VelocitySystem::accelerate(Velocity& vel, const float deltaTime) const {
	if (greater_e((vel.currentSpeed += vel.acceleration * deltaTime), vel.defaultSpeed))
		vel.currentSpeed = vel.defaultSpeed;
}

void VelocitySystem::decelerate(Velocity& vel, const float deltaTime) const {
	if (less_e((vel.currentSpeed -= vel.acceleration * deltaTime), vel.defaultSpeed))
		vel.currentSpeed = vel.defaultSpeed;
}