#pragma once

#include <vector>
#include <ent/Entities.hpp>
#include <memory>

struct GameContext {
	GameContext() = default;

	virtual ~GameContext() = default;

	virtual void init() = 0;
	virtual void update() = 0;

	[[nodiscard]] virtual const std::unique_ptr<EntityPlayer>& getPlayer() const = 0;
	[[nodiscard]] virtual 		std::unique_ptr<EntityPlayer>& getPlayer() 		 = 0;

	[[nodiscard]] virtual const std::unique_ptr<EntityCamera>& getCamera() const = 0;
	[[nodiscard]] virtual 		std::unique_ptr<EntityCamera>& getCamera() 		 = 0;

	[[nodiscard]] virtual const std::vector<std::unique_ptr<Entity>>& getEntities() const = 0;
	[[nodiscard]] virtual 		std::vector<std::unique_ptr<Entity>>& getEntities() 	  = 0;

	[[nodiscard]] virtual std::vector<Sun::SceneNode>& getSceneNodeComponents() = 0;
	[[nodiscard]] virtual std::vector<Sun::CameraNode>& getCameraNodeComponents() = 0;

	[[nodiscard]] virtual std::vector<BoundingBox>& getBoundingComponents() = 0;

	[[nodiscard]] virtual std::vector<Physics>& getPhysicsComponents() = 0;
};

// TODO: definir y actualizar periodicamente una interfaz correcta en funcion de las necesidades que surjan

