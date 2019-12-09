#pragma once

#include <ent/Entities.hpp>
#include <memory>
#include <vector>
#include <util/GameContext.hpp>
#include <SunlightEngine/Device.hpp>

struct EntityManager : GameContext {
	EntityManager() = default;
	explicit EntityManager(Device* device) : device(device) {}
	~EntityManager() override { cleanVectors(); }

	int init();
	void killPlayers();
	[[nodiscard]] const std::vector<Entity>& getEntities() const override { return entities; }
	[[nodiscard]] const std::vector<std::unique_ptr<EntityPlayer>>& getPlayers() const { return players; }

	void cleanVectors();

	private:
		Sun::Device* device { nullptr };

		std::vector<Entity> entities;
		std::vector<std::unique_ptr<EntityPlayer>> players;
		std::vector<std::unique_ptr<EntityCamera>> cameras;
};