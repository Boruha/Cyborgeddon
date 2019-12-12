#pragma once

#include <ent/Entities.hpp>

#include <memory>
#include <vector>
#include <util/GameContext.hpp>
#include <SunlightEngine/Device.hpp>

struct EntityManager : GameContext {
	explicit EntityManager(const Device& device) : device(device) {  }
	~EntityManager() override { cleanVectors(); }

	void init();
	void takeKey();
	void openDoor();

	void createPlayer (const Vector3f& pos = Vector3f(), const Vector3f& dim = Vector3f(15) , const float& speed = 1.f);
	void createEnemy  (const Vector3f& pos = Vector3f(), const Vector3f& dim = Vector3f(8) , const float& speed = 0.1f);
	void createCamera (const Vector3f& pos = Vector3f(), const Vector3f& target = Vector3f(0,0,100));
    void createDoor(const Vector3f& pos = Vector3f(), const Vector3f& dim = Vector3f(5));
    void createKey(const Vector3f& pos = Vector3f(), const Vector3f& dim = Vector3f(2));

	[[nodiscard]] EntityPlayer& getPlayer() override { return *player; }
	[[nodiscard]] const EntityCamera& getCamera() const override { return camera; }
	[[nodiscard]] const std::vector<std::unique_ptr<EntityEnemy>>&  getEnemies() const override { return enemies; }
	[[nodiscard]] const std::vector<std::unique_ptr<EntityDoor>>& getDoors() const override { return doors; }
	[[nodiscard]] const std::vector<std::unique_ptr<EntityKey>>& getKeys() const override { return keys; }

	private:
        void cleanVectors();

	    const Sun::Device& device;

		std::vector<Entity> entities;

		EntityPlayer* player { nullptr };
		EntityCamera camera { device };

		std::vector<std::unique_ptr<EntityEnemy>> enemies;
		std::vector<std::unique_ptr<EntityDoor>> doors;
		std::vector<std::unique_ptr<EntityKey>> keys;
};

// TODO: Player y camera solo uno de momento