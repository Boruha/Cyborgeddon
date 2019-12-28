#pragma once

#include <ent/Entities.hpp>

#include <memory>
#include <vector>
#include <util/GameContext.hpp>
#include <SunlightEngine/Device.hpp>
#include <src/cmp/Storage.hpp>

struct EntityManager : GameContext {
	explicit EntityManager(const Device& device) : device(device) {  }
	~EntityManager() override { cleanVectors(); }

	void init() override;
	void update() override;


	void createPairPlayerCamera (const int& health, const Vector3f& pos, const Vector3f& dim, const float& speed, const Vector3f& posCamera);
	void createEnemy  (const Vector3f& pos, const Vector3f& dim, const float& speed, const std::vector<Vector3f>& patrol = std::vector<Vector3f>());
    void createDoor   (const Lock& lock, const Vector3f& pos, const Vector3f& dim = Vector3f(5));
    void createWall   (const Vector3f& pos = Vector3f(), const Vector3f& dim = Vector3f(5));
    void createKey    (const Lock& lock, const Vector3f& pos, const Vector3f& dim = Vector3f(3));
	void createBullet (const Vector3f& dim);
    void createFloor  (const char* tex, const Vector3f& pos = Vector3f(), const Vector3f& dim = Vector3f(5));

    void createPairKeyDoor (const Vector3f& keyPos, const Vector3f& keyDim, const Vector3f& doorPos, const Vector3f& doorDim);


	[[nodiscard]] std::unique_ptr<EntityPlayer>& getPlayer() override { return player; }
	[[nodiscard]] std::unique_ptr<EntityCamera>& getCamera() override { return camera; }
	[[nodiscard]] const std::vector<std::unique_ptr<EntityEnemy>>&  getEnemies() const override { return enemies; }

	[[nodiscard]] const std::vector<std::unique_ptr<EntityDoor>>& 	  getDoors() const override { return 	doors; }
    [[nodiscard]] const std::vector<std::unique_ptr<EntityWall>>&     getWalls() const override { return 	walls; }
	[[nodiscard]] const std::vector<std::unique_ptr<EntityKey>>&       getKeys() const override { return 	 keys; }
	[[nodiscard]] const std::vector<std::unique_ptr<EntityBullet>>& getBullets() const override { return  bullets; }
	[[nodiscard]] const std::vector<std::unique_ptr<EntityFloor>>&    getFloor() const override { return    floor; }

	[[nodiscard]] std::vector<SceneNode>& getSceneNodeComponents() override { return componentStorage.getSceneNodeComponents(); }
	[[nodiscard]] std::vector<CameraNode>& getCameraNodeComponents() override { return componentStorage.getCameraNodeComponents(); }

	private:
        void cleanVectors();
		void takeKey();
		void openDoor();
		void checkShooting();
		void deleteBullet();
        void killEnemy();
        void killPlayer();

	    const Sun::Device& device;

		std::unique_ptr<EntityPlayer> player;
		std::unique_ptr<EntityCamera> camera;

		std::vector<std::unique_ptr<EntityEnemy>> enemies;

		std::vector<std::unique_ptr<EntityDoor>> doors;
        std::vector<std::unique_ptr<EntityWall>> walls;
		std::vector<std::unique_ptr<EntityKey>> keys;

		std::vector<std::unique_ptr<EntityBullet>> bullets;

		std::vector<std::unique_ptr<EntityFloor>> floor;

		Storage componentStorage { 512 }; // 512 por poner algo
};

// TODO: Player y camera solo uno de momento