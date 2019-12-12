#pragma once

#include <ent/Entities.hpp>
#include <sys/Systems.hpp>

struct AI_System
{
	void init() {  }
    void update(std::unique_ptr<EntityPlayer>& player, const std::vector<std::unique_ptr<EntityEnemy>>& enemies);
};
