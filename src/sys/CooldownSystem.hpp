#pragma once

#include <src/man/EntityManager.hpp>
#include <sys/System.hpp>

#include <ctime>

struct CooldownSystem : System {

    void init() override {  };
    void update(const std::unique_ptr<GameContext>& context, float deltaTime) const override;

    static bool dashReady();

    inline static std::clock_t generalClock;

    inline static float bulletClock;
    inline static float lastBullet;

    inline static float dashClock;
    inline static float lastDash;
};


