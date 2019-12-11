#pragma once

#include <ent/Entities.hpp>
#include <sys/Systems.hpp>

struct AI_System
{
	void init() {  }
    void update(EntityPlayer& player, const std::vector<std::unique_ptr<EntityEnemy>>& enemies);
};
