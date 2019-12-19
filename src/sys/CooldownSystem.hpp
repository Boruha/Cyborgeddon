#pragma once

#include <ent/Entities.hpp>
#include <src/man/EntityManager.hpp>
#include <sys/System.hpp>

#include <ctime>

struct CooldownSystem : System {

    void init() override { startTime = clock(); }
    void update(const std::unique_ptr<GameContext>& context) const override {  }

    static bool shootReady();

    inline static std::clock_t bulletClock { 0 };
    inline static std::clock_t startTime { 0 };
};
