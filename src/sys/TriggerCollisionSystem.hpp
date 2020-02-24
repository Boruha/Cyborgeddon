#pragma once

#include <sys/System.hpp>

struct TriggerCollisionSystem : System {
    void init() override;
    void update(const Context& context, float deltaTime) override;
    void reset() override;
};
