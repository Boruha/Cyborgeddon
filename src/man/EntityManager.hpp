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

	void createPlayer(const Vector3f& pos = Vector3f(), const Vector3f& dim = Vector3f(5) , const float& speed = 1.f);
    void createDoor(const Vector3f& pos = Vector3f(), const Vector3f& dim = Vector3f(10));
    void createKey(const Vector3f& pos = Vector3f(), const Vector3f& dim = Vector3f(2));
	void createCamera(const Vector3f& pos = Vector3f(), const Vector3f& target = Vector3f(0,0,100));

	[[nodiscard]] const std::vector<std::unique_ptr<EntityPlayer>>& getPlayers() const override { return players; }
    [[nodiscard]] const std::vector<std::unique_ptr<EntityDoor>>& getDoors() const  { return doors; }
    [[nodiscard]] const std::vector<std::unique_ptr<EntityKey>>& getKeys() const  { return keys; }
	[[nodiscard]] const std::vector<std::unique_ptr<EntityCamera>>& getCameras() const override { return cameras; }
	private:
        void cleanVectors();

	    const Sun::Device& device;

		std::vector<Entity> entities;
		std::vector<std::unique_ptr<EntityPlayer>> players;
        std::vector<std::unique_ptr<EntityDoor>> doors;
        std::vector<std::unique_ptr<EntityKey>> keys;
		std::vector<std::unique_ptr<EntityCamera>> cameras;
};