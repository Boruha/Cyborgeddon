#pragma once

#include <ent/Entities.hpp>

#include <vector>

struct EntityManager {
	explicit EntityManager(irr::IrrlichtDevice* device) : player(device), camera(device) {}
	~EntityManager() { entities.clear(); }

	int init();

	EntityPlayer player;
	EntityCamera camera;
	std::vector<Entity*> entities;
};