#pragma once

#include <ent/Entities.hpp>
#include <memory>
#include <vector>
#include <util/GameContext.hpp>
#include <SunlightEngine/Device.hpp>

struct EntityManager : GameContext {
	EntityManager() = delete;
	EntityManager(const EntityManager& em) = delete;
	explicit EntityManager(const Device& device) : device(device) {  }
	~EntityManager() override { cleanVectors(); }

	void init();
	void killPlayers();

	[[nodiscard]] const std::vector<std::unique_ptr<EntityPlayer>>& getPlayers() const override { return players; }
	[[nodiscard]] const std::vector<std::unique_ptr<EntityCamera>>& getCameras() const override { return cameras; }

	private:
        void cleanVectors();

	    const Sun::Device& device;

		std::vector<Entity> entities;
		std::vector<std::unique_ptr<EntityPlayer>> players;
		std::vector<std::unique_ptr<EntityCamera>> cameras;
};