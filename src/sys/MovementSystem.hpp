#pragma once

#include <ent/Entities.hpp>

#include <src/man/EntityManager.hpp>

struct MovementSystem
{
	MovementSystem() = default;

	void init() {  }

	void update(EntityPlayer&);
	void update(const std::vector<std::unique_ptr<EntityEnemy>>&);
};