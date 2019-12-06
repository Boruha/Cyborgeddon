#pragma once

#include <ent/Entities.hpp>

#include <vector>
#include <util/GameContext.hpp>

struct EntityManager : GameContext {
	EntityManager() = default;
	explicit EntityManager(irr::IrrlichtDevice* device) : device(device) {}
	~EntityManager() override { entities.clear(); }

	int init();
	[[nodiscard]] const std::vector<Entity>& getEntities() const override { return entities; }

	irr::IrrlichtDevice* device = { nullptr };

	EntityPlayer player {device};
	EntityCamera camera {device, Vector3f(player.node->getPosition().X,player.node->getPosition().Y,player.node->getPosition().Z)};

	private:
		std::vector<Entity> entities;
};