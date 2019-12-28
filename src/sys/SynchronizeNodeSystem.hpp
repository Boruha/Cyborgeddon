#pragma once

#include <sys/System.hpp>

struct SynchronizeNodeSystem : System {
	void init() override {  }
	void update(const std::unique_ptr<GameContext>& context) const;
};