#pragma once

#include <ent/Entities.hpp>

#include <src/man/EntityManager.hpp>

struct MovementSystem
{
	MovementSystem() = default;
	~MovementSystem() = default;

	void init() {  }
	void update(const std::vector<std::unique_ptr<EntityPlayer>>&);
	void update(const std::vector<std::unique_ptr<EntityEnemy>>&);
	void update(const std::vector<std::unique_ptr<EntityBullet>>&);
	void checkMaxDist_Bullet(const std::vector<std::unique_ptr<EntityBullet>>&);
};