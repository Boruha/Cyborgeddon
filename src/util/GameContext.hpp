#pragma once

#include <vector>
#include <ent/Entities.hpp>
#include <memory>

struct GameContext {
	GameContext() = default;

	virtual ~GameContext() = default;

	virtual void init() = 0;
	virtual void update() = 0;

	[[nodiscard]] virtual std::unique_ptr<EntityPlayer>& getPlayer() = 0;
	[[nodiscard]] virtual EntityCamera& getCamera() = 0;
	[[nodiscard]] virtual const std::vector<std::unique_ptr<EntityEnemy>>&  getEnemies() const = 0;

	[[nodiscard]] virtual const std::vector<std::unique_ptr<EntityDoor>>& getDoors() const = 0;
    [[nodiscard]] virtual const std::vector<std::unique_ptr<EntityWall>>& getWalls() const = 0;
	[[nodiscard]] virtual const std::vector<std::unique_ptr<EntityKey>>& getKeys() const = 0;
	[[nodiscard]] virtual const std::vector<std::unique_ptr<EntityBullet>>& getBullets() const = 0;
};

// TODO: definir y actualizar periodicamente una interfaz correcta en funcion de las necesidades que surjan

