#pragma once

#include <ent/Entities.hpp>

#include <memory>
#include <vector>
#include <util/GameContext.hpp>
#include <SunlightEngine/Device.hpp>
#include <src/cmp/Storage.hpp>

struct EntityManager : GameContext {
	explicit EntityManager(const Device& device) : device(device) {  } // mientras player y camera sean independientes hay que eliminar sus nodos manualmente
	~EntityManager() override { cleanVectors(); }

	void init() override;
	void update() override;

	void checkShooting();
	void addToDestroy(std::size_t ID) override;

	void createPairPlayerCamera (const int& health, const Vector3f& pos, const Vector3f& dim, const float& speed, const Vector3f& posCamera);
	void createEnemy  (const Vector3f& pos, const Vector3f& dim, const float& speed, const std::vector<Vector3f>& patrol = std::vector<Vector3f>());
    void createWall   (const Vector3f& pos = Vector3f(), const Vector3f& dim = Vector3f(5));
	void createBullet (const Vector3f& dim);
    void createFloor  (const char* tex, const Vector3f& pos = Vector3f(), const Vector3f& dim = Vector3f(5));

    void createPairKeyDoor (const Vector3f& keyPos, const Vector3f& keyDim, const Vector3f& doorPos, const Vector3f& doorDim);


	[[nodiscard]] const Entity& getPlayer() const override { return *player; }
	[[nodiscard]] 		Entity& getPlayer() 	  override { return *player; }
	[[nodiscard]] const Entity& getCamera() const override { return *camera; }
	[[nodiscard]] 		Entity& getCamera() 	  override { return *camera; }

	[[nodiscard]] const std::vector<Entity>& getEntities() const override { return entities; }
	[[nodiscard]] 		std::vector<Entity>& getEntities() 	  override { return entities; }

	[[nodiscard]] std::vector<Node_ptr>& getNodeComponents() override { return componentStorage.getNodeComponents(); }

	[[nodiscard]] vector<BoundingBox>& getStaticBoundingComponents() override { return componentStorage.getStaticBoundingComponents(); }

	[[nodiscard]] std::vector<BoundingBox>& getBoundingComponents() override { return componentStorage.getBoundingComponents(); }

	[[nodiscard]] std::vector<Physics>& getPhysicsComponents() override { return componentStorage.getPhysicsComponents(); }

	[[nodiscard]] std::vector<Velocity>& getVelocityComponents() override { return componentStorage.getVelocityComponents(); }

	[[nodiscard]] std::vector<BulletData>& getBulletDataComponents() override { return componentStorage.getBulletDataComponents(); }

	[[nodiscard]] std::vector<CharacterData>& getCharacterDataComponents() override { return componentStorage.getCharacterDataComponents(); }

	private:
        void cleanVectors();
		void killEntities();
		void moveDeadEntities();
		void removeEntities();

		Entity* player { nullptr };
		Entity* camera { nullptr };

	    const Sun::Device& device;

	    std::vector<std::size_t> toDelete;

	    std::vector<Entity> entities;

		Storage componentStorage { 512 }; // 512 por poner algo
};

// TODO: Player y camera solo uno de momento