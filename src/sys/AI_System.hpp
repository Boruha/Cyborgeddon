#pragma once

#include <ent/Entities.hpp>
#include <sys/Systems.hpp>

struct AI_System
{
	void init() {  }
    void update(const std::unique_ptr<EntityPlayer>&, const std::vector<std::unique_ptr<EntityEnemy>>&);
};
