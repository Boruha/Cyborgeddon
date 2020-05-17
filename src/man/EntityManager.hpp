#pragma once

#include <util/GameContext.hpp>
#include <src/cmp/Storage.hpp>

struct IEngine;

struct aiMesh;
struct aiScene;
struct aiNode;

struct EntityManager : GameContext {
	explicit EntityManager(IEngine * const _engine) : engine(*_engine), componentStorage(_engine) {  } // mientras player y camera sean independientes hay que eliminar sus nodos manualmente
	~EntityManager() override = default;

	void nextVideo() override;

	void createIntro() override;
	void createTutorial() override;

	void createLevel() override;
	void createGraph() override;
	void createBullet () override;

	bool checkVictory() const override;
	bool checkDefeat() const override;

	void addToDestroy(EntityID ID) override;

	[[nodiscard]] bool isKeyPressed(const unsigned key) const override { return engine.isKeyPressed(key); };
	[[nodiscard]] bool isKeyTextInput(const unsigned key) const override { return engine.isKeyTextInput(key); };

	[[nodiscard]] const Mouse& getMouse()   const override { return engine.getMouse(); };
	[[nodiscard]] vec3 getWorldPosFromCursor(const float x, const float y, const float depth) const override { return engine.scene->cursorToWorld(x, y, depth); };
	[[nodiscard]] IEngine& getEngine() const override { return engine; };

	[[nodiscard]] const Entity& getPlayer() const override { return *player; }
	[[nodiscard]] 		Entity& getPlayer() 	  override { return *player; }

	[[nodiscard]] const Entity& getCamera() const override { return *camera; }
	[[nodiscard]] 		Entity& getCamera() 	  override { return *camera; }

	[[nodiscard]] const Entity& getLight() const override { return *light; }
	[[nodiscard]] 		Entity& getLight() 	     override { return *light; }

	[[nodiscard]] const std::vector<MapNode>& getGraph() const override { return graph; }
	[[nodiscard]] 		std::vector<MapNode>& getGraph() 	   override { return graph; }

	[[nodiscard]] const std::unordered_map<EntityID, std::vector<int>>& getPaths() const override { return paths; }
	[[nodiscard]] 		std::unordered_map<EntityID, std::vector<int>>& getPaths() 	     override { return paths; }

	[[nodiscard]]       std::vector<int>& getPath(EntityID eid) 		  override { return paths.find(eid)->second; }
				        void deletePath(EntityID eid) 			          override { paths.erase(eid);  }
				  		void setPath(EntityID eid, std::vector<int> path) override { paths[eid] = path; }

	[[nodiscard]] const Entity& getEntityByID(EntityID id) const override;
	[[nodiscard]] 		Entity& getEntityByID(EntityID id) 		 override;

	[[nodiscard]] const Storage& getComponents() const override { return componentStorage; }
	[[nodiscard]] 		Storage& getComponents() 	   override { return componentStorage; }

	[[nodiscard]] unsigned getVideoIndex() const override { return idxVideo; }

	private:

		void initData(int maxEntities, int maxToDelete, int maxComponents);
        void cleanData();

        Entity& createEntity(EntityType type);

		void createPairPlayerCamera(const vec3& pos, const vec3& dim, const vec3& posCamera);

		void createEnemy(const vec3& pos, const vec3& dim, const std::vector<vec3>& patrol, unsigned phase);
		void createAngel(const vec3& pos, const vec3& dim, const std::vector<vec3>& patrol, unsigned phase);
		void createDemon(const vec3& pos, const vec3& dim, const std::vector<vec3>& patrol, unsigned phase);
		
		void createLight(const vec3& pos, const vec3& dir, const vec3& diff, const vec3& spe);
		void createWall (const vec3& pos, const vec3& dim);
		void createFloor(std::string_view tex, const vec3& pos, const vec3& dim);

		void createPairKeyDoor(const vec3& keyPos, const vec3& keyDim, const vec3& doorPos, const vec3& doorDim);
		//void setNavConnections(const GraphNode& node, const std::vector<const GraphNode*>& conn) const;
		//void createNavigation();

		void createVideo(std::string_view, bool, SoundParameter);
		void createTexture(std::string_view, int, int);

		void createMenuOptions(unsigned, unsigned);

		// lector de colliders
		void processNode(aiNode * node, const aiScene * scene);
		void processMesh(aiMesh * mesh);
		void readColliderFile(std::string_view);
		// lector de colliders

		Entity * player { nullptr };
		Entity * camera { nullptr };
		Entity * light  { nullptr };
		//Entity * nav    { nullptr };

		std::vector<MapNode> graph {};
		std::unordered_map<EntityID, std::vector<int>> paths {};

		unsigned idxVideo {0};

		IEngine& engine;

	    std::vector<EntityID> toDelete {};
	    std::unordered_map<EntityID, Entity> entities {};

		int enemiesLeft { 0 };	// de momento esta es la condicion de "victoria" que nos hace pasar (reiniciar en este caso) de nivel

		Storage componentStorage;
};

// TODO: Player y camera solo uno de momento