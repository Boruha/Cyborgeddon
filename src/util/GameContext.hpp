#pragma once

#include <vector>
#include <ent/Entity.hpp>

struct GameContext {
	GameContext() = default;

	virtual ~GameContext() = default;

	[[nodiscard]] virtual EntityPlayer& getPlayer() = 0;
	[[nodiscard]] virtual const EntityCamera& getCamera() const = 0;
	[[nodiscard]] virtual const std::vector<std::unique_ptr<EntityEnemy>>&  getEnemies() const = 0;
};

// TODO: definir y actualizar periodicamente una interfaz correcta en funcion de las necesidades que surjan