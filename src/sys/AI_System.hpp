#pragma once

#include <ent/Entities.hpp>
#include <sys/Systems.hpp>

struct AI_System
{
    void update(EntityPlayer& player, EntityEnemy& enemy);
};
