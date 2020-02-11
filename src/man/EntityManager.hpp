#pragma once

#include <util/GameContext.hpp>
#include <src/cmp/Storage.hpp>
#include <ent/Graph.hpp>


struct IEngine;

struct EntityManager : GameContext {
	explicit EntityManager(const IEngine * const engine) : engine(engine), componentStorage(engine) {  } // mientras player y camera sean independientes hay que eliminar sus nodos manualmente
	~EntityManager() override { cleanData(); }

	void init() override;
	bool update() override;

	void createLevel() override;
	void createBullet () override;

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

		void createPairPlayerCamera (const vec3& pos, const vec3& dim, const vec3& posCamera);
		void createEnemy  (const vec3& pos, const vec3& dim, const std::vector<vec3>& patrol);
		void createWall   (const vec3& pos, const vec3& dim);
		void createFloor  (const char* tex, const vec3& pos, const vec3& dim);
		void createPairKeyDoor (const vec3& keyPos, const vec3& keyDim, const vec3& doorPos, const vec3& doorDim);

		void killEntities();
		void moveDeadEntities();
		void removeEntities();

		bool checkVictory();

		Entity* player { nullptr };
		Entity* camera { nullptr };

	    const IEngine * const engine { nullptr };

	    std::vector<std::size_t> toDelete;
	    std::vector<Entity> entities;
		unsigned int entitiesLeftToDelete { 0 };

		unsigned int enemiesLeft { 0 };	// de momento esta es la condicion de "victoria" que nos hace pasar (reiniciar en este caso) de nivel

		Storage componentStorage;
};

// TODO: Player y camera solo uno de momento