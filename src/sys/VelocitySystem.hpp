#pragma once

#include <sys/System.hpp>

struct VelocitySystem : System {
	void init() override {  }
	void update(const std::unique_ptr<GameContext>& context) const override;

private:
	void accelerate(Velocity& vel) const;
	void decelerate(Velocity& vel) const;
};