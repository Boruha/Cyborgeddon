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
	void update();

	void createPlayer (const Vector3f& pos = Vector3f(), const Vector3f& dim = Vector3f(12) , const float& speed = 1.f);
	void createEnemy  (const Vector3f& pos = Vector3f(), const Vector3f& dim = Vector3f(8) , const float& speed = 0.5f);
	void createCamera (const Vector3f& pos = Vector3f(), const Vector3f& target = Vector3f(0,0,100));
	void createBullet (const Vector3f& pos = Vector3f(), const Vector3f& dir = Vector3f(), const bool& type = false);

	[[nodiscard]] const std::vector<std::unique_ptr<EntityPlayer>>& getPlayers() const override { return players; }
	[[nodiscard]] const std::vector<std::unique_ptr<EntityEnemy>>&  getEnemies() const override { return enemies; }
	[[nodiscard]] const std::vector<std::unique_ptr<EntityCamera>>& getCameras() const override { return cameras; }
	[[nodiscard]] const std::vector<std::unique_ptr<EntityBullet>>& getBullets() const override { return bullets; }

	private:
        void cleanVectors();
		void checkShooting();
		void deleteBullet();

	    const Sun::Device& device;

		std::vector<Entity> entities;
		std::vector<std::unique_ptr<EntityPlayer>> players;
		std::vector<std::unique_ptr<EntityEnemy>> enemies;
		std::vector<std::unique_ptr<EntityCamera>> cameras;
		std::vector<std::unique_ptr<EntityBullet>> bullets;
};

// TODO: Player y camera solo uno de momento