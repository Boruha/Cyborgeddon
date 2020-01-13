#pragma once

#include <vector>
#include <ent/Entity.hpp>
#include <memory>
#include <SunlightEngine/CameraNode.hpp>

struct GameContext {
	GameContext() = default;

	virtual ~GameContext() = default;

	virtual void init() = 0;
	virtual void update() = 0;

	[[nodiscard]] virtual const Entity& getPlayer() const = 0;
	[[nodiscard]] virtual 		Entity& getPlayer()		  = 0;

	[[nodiscard]] virtual const Entity& getCamera() const = 0;
	[[nodiscard]] virtual 		Entity& getCamera() 	  = 0;

	[[nodiscard]] virtual const std::vector<Entity>& getEntities() const = 0;
	[[nodiscard]] virtual 		std::vector<Entity>& getEntities() 	  	 = 0;

	[[nodiscard]] virtual std::vector<Node_ptr>& getNodeComponents() = 0;

	[[nodiscard]] virtual std::vector<BoundingBox>& getStaticBoundingComponents() = 0;

	[[nodiscard]] virtual std::vector<BoundingBox>& getBoundingComponents() = 0;

	[[nodiscard]] virtual std::vector<Physics>& getPhysicsComponents() = 0;

	[[nodiscard]] virtual std::vector<Velocity>& getVelocityComponents() = 0;

	[[nodiscard]] virtual std::vector<BulletData>& getBulletDataComponents() = 0;

	[[nodiscard]] virtual std::vector<CharacterData>& getCharacterDataComponents() = 0;

	virtual void addToDestroy(std::size_t ID) = 0;
};

// TODO: definir y actualizar periodicamente una interfaz correcta en funcion de las necesidades que surjan

