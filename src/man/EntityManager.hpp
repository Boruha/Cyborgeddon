#pragma once

#include <ent/Entities.hpp>

#include <vector>

struct EntityManager {
	explicit EntityManager(irr::IrrlichtDevice* device) : device(device) {}
	~EntityManager() { entities.clear(); }

	int init();

	irr::IrrlichtDevice* device = { nullptr };

	EntityPlayer player {device};
	EntityCamera camera {device};
	EntityDoor door {device};
    EntityKey key {device};

	std::vector<Entity*> entities;
};