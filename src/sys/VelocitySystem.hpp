#pragma once

#include <sys/System.hpp>

struct VelocitySystem : System {

	~VelocitySystem() override = default;

	void init() override {  }
	void fixedUpdate(const Context &context, float deltaTime) override;
	void update(const Context &context, float deltaTime) override {};
	void reset() override {  }

private:
	constexpr void accelerate(Velocity& vel, float deltaTime) const;
	constexpr void decelerate(Velocity& vel, float deltaTime) const;
};