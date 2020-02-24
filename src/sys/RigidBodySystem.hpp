#pragma once

#include <sys/System.hpp>

struct RigidBodySystem : System {
	void init() override {  }
	void update(const Context& context, float deltaTime);
	void reset() override {  }
};