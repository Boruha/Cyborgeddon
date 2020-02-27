#pragma once

#include <util/GameContext.hpp>
#include <src/cmp/Storage.hpp>
#include <map>

struct IEngine;

struct EntityManager : GameContext {
	explicit EntityManager(const IEngine * const _engine) : engine(*_engine), componentStorage(_engine) {  } // mientras player y camera sean independientes hay que eliminar sus nodos manualmente
	~EntityManager() override = default;

	void init() override;
	bool update() override;

	void createLevel() override;
	void createGraph() override;
	void createBullet () override;

	void addToDestroy(EntityID ID) override;

	[[nodiscard]] const Entity& getPlayer() const override { return *player; }
	[[nodiscard]] 		Entity& getPlayer() 	  override { return *player; }
	[[nodiscard]] const Entity& getCamera() const override { return *camera; }
	[[nodiscard]] 		Entity& getCamera() 	  override { return *camera; }

	[[nodiscard]] const std::vector<MapNode>& getGraph() const override { return graph; }
	[[nodiscard]] 		std::vector<MapNode>& getGraph() 	   override { return graph; }

	[[nodiscard]]       std::vector<int>& getPath(EntityID eid) 		  override { return paths[eid]; }
				        void deletePath(EntityID eid) 			          override { paths.erase(eid);  }
				  		void setPath(EntityID eid, std::vector<int> path) override { paths[eid] = path; }

	[[nodiscard]] const Entity& getEntityByID(EntityID id) const override;
	[[nodiscard]] 		Entity& getEntityByID(EntityID id) 		 override;

	[[nodiscard]] const ComponentPool& getComponents() const override { return componentStorage.getComponents(); }
	[[nodiscard]] 		ComponentPool& getComponents() 	     override { return componentStorage.getComponents(); }

	[[nodiscard]] const vector<Node_ptr>& getNodes()   const override { return componentStorage.getNodes(); }
	[[nodiscard]]       vector<Node_ptr>& getNodes()         override { return componentStorage.getNodes(); }

	private:

		void initData(int maxEntities, int maxToDelete, int maxComponents);
        void cleanData();

        Entity& createEntity(EntityType type);

		void createPairPlayerCamera (const vec3& pos, const vec3& dim, const vec3& posCamera);
		void createEnemy  (const vec3& pos, const vec3& dim, const std::vector<vec3>& patrol);
		void createWall   (const vec3& pos, const vec3& dim);
		void createFloor  (const char* tex, const vec3& pos, const vec3& dim);
		void createPairKeyDoor (const vec3& keyPos, const vec3& keyDim, const vec3& doorPos, const vec3& doorDim);

		void killEntities();

		bool checkVictory();

		Entity * player { nullptr };
		Entity * camera { nullptr };

    	std::vector<MapNode> graph;
		std::map<EntityID, std::vector<int>> paths;

	    const IEngine& engine;

	    std::vector<EntityID> toDelete;
	    std::unordered_map<EntityID, Entity> map_entities;

		unsigned int entitiesLeftToDelete { 0 };

		unsigned int enemiesLeft { 0 };	// de momento esta es la condicion de "victoria" que nos hace pasar (reiniciar en este caso) de nivel

		Storage componentStorage;
};

// TODO: Player y camera solo uno de momento