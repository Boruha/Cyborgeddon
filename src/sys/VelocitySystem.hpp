#pragma once

#include <sys/System.hpp>

struct VelocitySystem : System {
	void init() override {  }
	void update(const Context &context, float deltaTime) override;
	void reset() override {  }

private:
	void accelerate(Velocity& vel, float deltaTime) const;
	void decelerate(Velocity& vel, float deltaTime) const;
};