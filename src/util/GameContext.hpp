#pragma once

#include <util/ComponentType.hpp>
#include <cmp/ComponentVariant.hpp>
#include <ent/Entity.hpp>

struct GameContext {
	GameContext() = default;

	virtual ~GameContext() = default;

	virtual void init() = 0;
	virtual bool update() = 0;

	virtual void createLevel() = 0;

	[[nodiscard]] virtual const Entity& getPlayer() const = 0;
	[[nodiscard]] virtual 		Entity& getPlayer()		  = 0;

	[[nodiscard]] virtual const Entity& getCamera() const = 0;
	[[nodiscard]] virtual 		Entity& getCamera() 	  = 0;

	[[nodiscard]] virtual const std::vector<Entity>& getEntities() const = 0;
	[[nodiscard]] virtual 		std::vector<Entity>& getEntities() 	  	 = 0;

	[[nodiscard]] virtual const Entity& getEntityByID(std::size_t id) const = 0;
	[[nodiscard]] virtual const Entity& getEntityByID(std::size_t id) 	    = 0;

	[[nodiscard]] virtual const variantComponentVectorTypes& getComponents(ComponentType type) const = 0;
	[[nodiscard]] virtual 		variantComponentVectorTypes& getComponents(ComponentType type) 		 = 0;

	virtual void addToDestroy(std::size_t ID) = 0;
};

// TODO: definir y actualizar periodicamente una interfaz correcta en funcion de las necesidades que surjan

