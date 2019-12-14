#pragma once

#include <ent/Entities.hpp>
#include <sys/Systems.hpp>

struct ModeSystem
{
	void init();
    void update(const std::unique_ptr<EntityPlayer>&);
};
