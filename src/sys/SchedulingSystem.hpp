#pragma once

#include <sys/System.hpp>

struct SchedulingSystem : System
{
    ~SchedulingSystem() override = default;

	void init()  override {  }
	void reset() override {  }

    void update(const Context &context, float deltaTime) override;
};
