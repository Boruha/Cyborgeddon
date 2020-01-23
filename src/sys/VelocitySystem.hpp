#pragma once

#include <sys/System.hpp>

struct VelocitySystem : System {
	void init() override {  }
	void update(const std::unique_ptr<GameContext> &context, float deltaTime) const override;
	void reset() override {  }

private:
	void accelerate(Velocity& vel, float deltaTime) const;
	void decelerate(Velocity& vel, float deltaTime) const;
};