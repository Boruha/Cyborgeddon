#pragma once

#include <ent/Entity.hpp>
#include <ent/Graph.hpp>
#include <Engine/util/Enum.hpp>
#include <src/Engine/util/Color.hpp>
#include <memory>

struct Storage;
struct Mouse;
struct IEngine;

struct GameContext {
	virtual ~GameContext() = default;

	virtual void createIntro()  = 0;
	virtual void createLevel()  = 0;
	virtual void createGraph()  = 0;
	virtual void createBullet() = 0;

	virtual void nextVideo() = 0;

	[[nodiscard]] virtual bool checkVictory() const = 0;
	[[nodiscard]] virtual bool checkDefeat()  const = 0;

	[[nodiscard]] virtual const Entity& getPlayer() const = 0;
	[[nodiscard]] virtual 		Entity& getPlayer()		  = 0;

	[[nodiscard]] virtual const Entity& getCamera() const = 0;
	[[nodiscard]] virtual 		Entity& getCamera() 	  = 0;

	[[nodiscard]] virtual const Entity& getLight() const = 0;
	[[nodiscard]] virtual 		Entity& getLight() 	     = 0;

	[[nodiscard]] virtual const std::vector<MapNode>& getGraph() const = 0;
	[[nodiscard]] virtual 		std::vector<MapNode>& getGraph()	   = 0;

	[[nodiscard]] virtual const std::unordered_map<EntityID, std::vector<int>>& getPaths() const = 0;
	[[nodiscard]] virtual       std::unordered_map<EntityID, std::vector<int>>& getPaths() 	     = 0;

	[[nodiscard]] virtual       std::vector<int>& getPath(EntityID)      = 0;
				  virtual       void deletePath(EntityID)                = 0;
				  virtual 		void setPath(EntityID, std::vector<int>) = 0;

	[[nodiscard]] virtual const Entity& getEntityByID(EntityID) const = 0;
	[[nodiscard]] virtual  		Entity& getEntityByID(EntityID) 	  = 0;

	[[nodiscard]] virtual const Storage& getComponents() const = 0;
	[[nodiscard]] virtual 		Storage& getComponents() 	   = 0;

	[[nodiscard]] virtual unsigned getVideoIndex() const = 0;

	virtual void addToDestroy(EntityID) = 0;

	[[nodiscard]] virtual bool isKeyPressed(unsigned) const = 0;
	[[nodiscard]] virtual bool isKeyTextInput(unsigned) const = 0;
	[[nodiscard]] virtual const Mouse& getMouse() const = 0;
	[[nodiscard]] virtual vec3 getWorldPosFromCursor(float, float, float) const = 0;

	[[nodiscard]] virtual IEngine & getEngine() const = 0;
};

// TODO: definir y actualizar periodicamente una interfaz correcta en funcion de las necesidades que surjan

using Context = std::unique_ptr<GameContext>;