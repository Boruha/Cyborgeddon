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

	void checkShooting();
	void checkEntitiesToDestroy();

	void createPairPlayerCamera (const int& health, const Vector3f& pos, const Vector3f& dim, const float& speed, const Vector3f& posCamera);
	void createEnemy  (const Vector3f& pos, const Vector3f& dim, const float& speed, const std::vector<Vector3f>& patrol = std::vector<Vector3f>());
    void createWall   (const Vector3f& pos = Vector3f(), const Vector3f& dim = Vector3f(5));
	void createBullet (const Vector3f& dim);
    void createFloor  (const char* tex, const Vector3f& pos = Vector3f(), const Vector3f& dim = Vector3f(5));

    void createPairKeyDoor (const Vector3f& keyPos, const Vector3f& keyDim, const Vector3f& doorPos, const Vector3f& doorDim);


	[[nodiscard]] const std::unique_ptr<EntityPlayer>& getPlayer() const override { return player; }
	[[nodiscard]] 		std::unique_ptr<EntityPlayer>& getPlayer() 		 override { return player; }
	[[nodiscard]] const std::unique_ptr<EntityCamera>& getCamera() const override { return camera; }
	[[nodiscard]] 		std::unique_ptr<EntityCamera>& getCamera() 		 override { return camera; }

	[[nodiscard]] const std::vector<std::unique_ptr<Entity>>& getEntities() const override { return entities; }
	[[nodiscard]] 		std::vector<std::unique_ptr<Entity>>& getEntities() 	  override { return entities; }

	[[nodiscard]] std::vector<SceneNode>& getSceneNodeComponents() override { return componentStorage.getSceneNodeComponents(); }
	[[nodiscard]] std::vector<CameraNode>& getCameraNodeComponents() override { return componentStorage.getCameraNodeComponents(); }

	[[nodiscard]] std::vector<BoundingBox>& getBoundingComponents() override { return componentStorage.getBoundingComponents(); };

	[[nodiscard]] std::vector<Physics>& getPhysicsComponents() override { return componentStorage.getPhysicsComponents(); };

	private:
        void cleanVectors();

	    const Sun::Device& device;

	    std::vector<std::unique_ptr<Entity>> entities;

		std::unique_ptr<EntityPlayer> player;
		std::unique_ptr<EntityCamera> camera;

		Storage componentStorage { 512 }; // 512 por poner algo
};

// TODO: Player y camera solo uno de momento