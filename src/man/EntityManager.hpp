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
	[[nodiscard]] 		std::vector<Entity>& getEntities() 	  	 override { return entities; }

	[[nodiscard]] const Entity& getEntityByID(std::size_t id) const override;
	[[nodiscard]] const Entity& getEntityByID(std::size_t id) 		override;

	[[nodiscard]] variantComponentVectorTypes& getComponents(ComponentType type) override { return componentStorage.getComponents(type); }

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
		int entitiesLeftToDelete { 0 };

		Storage componentStorage { 256 }; // 512 por poner algo
};

// TODO: Player y camera solo uno de momento