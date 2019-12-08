#pragma once

#include <ent/Entities.hpp>
#include <memory>
#include <vector>
#include <util/GameContext.hpp>

struct EntityManager : GameContext {
	EntityManager() = default;
	explicit EntityManager(irr::IrrlichtDevice* device) : device(device) {}
	~EntityManager() override { entities.clear(); }

	int init();
	void killPlayers();
	[[nodiscard]] const std::vector<Entity>& getEntities() const override { return entities; }
	[[nodiscard]] const std::vector<std::unique_ptr<EntityPlayer>>& getPlayers() const { return players; }

	irr::IrrlichtDevice* device = { nullptr };

	EntityCamera camera {device, Vector3f()};

	private:
		std::vector<Entity> entities;
		std::vector<std::unique_ptr<EntityPlayer>> players;
};