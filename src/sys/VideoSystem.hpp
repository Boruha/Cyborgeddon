#pragma once

#include <sys/System.hpp>

#include <chrono>

struct VideoSystem : System {

	~VideoSystem() override = default;

	void init() override {  }
	void fixedUpdate(const Context &context, float deltaTime) override;
	void update(const Context &context, float deltaTime) override;
	void reset() override {  }

	std::chrono::time_point<std::chrono::high_resolution_clock> time = std::chrono::time_point<std::chrono::high_resolution_clock>(std::chrono::nanoseconds::zero());
};