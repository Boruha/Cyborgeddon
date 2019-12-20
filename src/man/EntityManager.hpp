#pragma once

#include <ent/Entities.hpp>

#include <memory>
#include <vector>
#include <util/GameContext.hpp>
#include <SunlightEngine/Device.hpp>

struct EntityManager : GameContext {
	explicit EntityManager(const Device& device) : device(device) {  }
	~EntityManager() override { cleanVectors(); }

	void init() override;
	void update() override;


	void createPlayer (int health, const Vector3f& pos = Vector3f(), const Vector3f& dim = Vector3f(15) , const float& speed = 1.f);
	void createEnemy  (const Vector3f& pos = Vector3f(), const Vector3f& dim = Vector3f(8) , const float& speed = 0.1f);
	void createCamera (const Vector3f& pos = Vector3f(), const Vector3f& target = Vector3f(0,0,10));
    void createDoor(const int& type, const Vector3f& pos = Vector3f(), const Vector3f& dim = Vector3f(5));
    void createWall(const Vector3f& pos = Vector3f(), const Vector3f& dim = Vector3f(5));
    void createFloor(const Vector3f& pos = Vector3f(), const Vector3f& dim = Vector3f(5));
    void createKey(const int& type, const Vector3f& pos = Vector3f(), const Vector3f& dim = Vector3f(3));
	void createBullet (const Vector3f& pos = Vector3f(), Vector3f dir = Vector3f(), const bool& type = false, const Vector3f& dim = Vector3f(3));


	[[nodiscard]] std::unique_ptr<EntityPlayer>& getPlayer() override { return entities[0]; }
	[[nodiscard]] EntityCamera& getCamera() override { return camera; }
	[[nodiscard]] const std::vector<std::unique_ptr<EntityEnemy>>&  getEnemies() const override { return enemies; }

	[[nodiscard]] const std::vector<std::unique_ptr<EntityDoor>>& getDoors() const override { return doors; }
    [[nodiscard]] const std::vector<std::unique_ptr<EntityWall>>& getWalls() const override{ return walls; }
	[[nodiscard]] const std::vector<std::unique_ptr<EntityKey>>& getKeys() const override { return keys; }
	[[nodiscard]] const std::vector<std::unique_ptr<EntityBullet>>& getBullets() const override { return bullets; }

	private:
        void cleanVectors();
		void takeKey();
		void openDoor();
		void checkShooting();
		void deleteBullet();
        void killEnemy();
        void killPlayer();

	    const Sun::Device& device;

		std::vector<std::unique_ptr<EntityPlayer>> entities; // SOLO CONTIENE EL PLAYER POR AHORA
        std::vector<std::unique_ptr<EntityFloor>> floor;
		EntityCamera camera { device };

		std::vector<std::unique_ptr<EntityEnemy>> enemies;

		std::vector<std::unique_ptr<EntityDoor>> doors;
        std::vector<std::unique_ptr<EntityWall>> walls;
		std::vector<std::unique_ptr<EntityKey>> keys;

		std::vector<std::unique_ptr<EntityBullet>> bullets;
};

// TODO: Player y camera solo uno de momento