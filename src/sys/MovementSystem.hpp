#pragma once

#include <ent/Entities.hpp>

#include <src/man/EntityManager.hpp>

struct MovementSystem
{
	void update(const std::vector<std::unique_ptr<EntityPlayer>>&);
};