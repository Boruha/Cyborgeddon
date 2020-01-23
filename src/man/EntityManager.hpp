#pragma once

#include <memory>
#include <vector>
#include <util/GameContext.hpp>
#include <SunlightEngine/Device.hpp>
#include <src/cmp/Storage.hpp>

struct EntityManager : GameContext {
	explicit EntityManager(const Device& device) : device(device) {  } // mientras player y camera sean independientes hay que eliminar sus nodos manualmente
	~EntityManager() override { cleanData(); }

	void init() override;
	bool update() override;

	void createLevel() override;

	void addToDestroy(std::size_t ID) override;

	[[nodiscard]] const Entity& getPlayer() const override { return *player; }
	[[nodiscard]] 		Entity& getPlayer() 	  override { return *player; }
	[[nodiscard]] const Entity& getCamera() const override { return *camera; }
	[[nodiscard]] 		Entity& getCamera() 	  override { return *camera; }

	[[nodiscard]] const std::vector<Entity>& getEntities() const override { return entities; }
	[[nodiscard]] 		std::vector<Entity>& getEntities() 	  	 override { return entities; }

	[[nodiscard]] const Entity& getEntityByID(std::size_t id) const override;
	[[nodiscard]] const Entity& getEntityByID(std::size_t id) 		override;

	[[nodiscard]] const variantComponentVectorTypes& getComponents(ComponentType type) const override { return componentStorage.getComponents(type); }
	[[nodiscard]] 		variantComponentVectorTypes& getComponents(ComponentType type) 		 override { return componentStorage.getComponents(type); }

	private:

		void initData(int maxEntities, int maxToDelete, int maxComponents);
        void cleanData();

		void createPairPlayerCamera (const Vector3f& pos, const Vector3f& dim, const Vector3f& posCamera);
		void createEnemy  (const Vector3f& pos, const Vector3f& dim, const std::vector<Vector3f>& patrol);
		void createWall   (const Vector3f& pos, const Vector3f& dim);
		void createBullet (const Vector3f& dim);
		void createFloor  (const char* tex, const Vector3f& pos, const Vector3f& dim);
		void createPairKeyDoor (const Vector3f& keyPos, const Vector3f& keyDim, const Vector3f& doorPos, const Vector3f& doorDim);

		void killEntities();
		void moveDeadEntities();
		void removeEntities();

		void checkShooting();
		bool checkVictory();

		Entity* player { nullptr };
		Entity* camera { nullptr };

	    const Sun::Device& device;

	    std::vector<std::size_t> toDelete;
	    std::vector<Entity> entities;
		unsigned int entitiesLeftToDelete { 0 };

		unsigned int enemiesLeft { 0 };	// de momento esta es la condicion de "victoria" que nos hace pasar (reiniciar en este caso) de nivel

		Storage componentStorage;
};

// TODO: Player y camera solo uno de momento