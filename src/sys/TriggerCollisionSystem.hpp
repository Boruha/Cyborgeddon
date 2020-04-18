#pragma once

#include <sys/System.hpp>

struct TriggerCollisionSystem : System {

	~TriggerCollisionSystem() override = default;

    void init() override;
    void fixedUpdate(const Context& context, float deltaTime) override;
	void update(const Context &context, float deltaTime) override {};
    void reset() override;
};
