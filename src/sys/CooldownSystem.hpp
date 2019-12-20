#pragma once

#include <ent/Entities.hpp>
#include <src/man/EntityManager.hpp>
#include <sys/System.hpp>

#include <ctime>

struct CooldownSystem : System {

    void init() override {  };
    void update(const std::unique_ptr<GameContext>& context) const override {  }

    static bool shootReady();
    static bool dashReady();

    inline static std::clock_t generalClock;

    inline static float bulletClock;
    inline static float lastBullet;

    inline static float dashClock;
    inline static float lastDash;
};


