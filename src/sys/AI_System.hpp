#pragma once

#include <sys/System.hpp>
#include <ent/Entities.hpp>
#include <util/GameContext.hpp>

struct AI_System : System
{
	void init() override {  }
    void update(const std::unique_ptr<GameContext>& context) const override;
};
