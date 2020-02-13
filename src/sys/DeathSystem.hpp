#pragma once

#include <sys/System.hpp>

struct DeathSystem : System {

    void init() override {  }
    void update(const std::unique_ptr<GameContext>& context, float deltaTime) override;
    void reset() override {  }

};