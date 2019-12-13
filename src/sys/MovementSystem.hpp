#pragma once

#include <ent/Entities.hpp>

#include <src/man/EntityManager.hpp>

struct MovementSystem
{
	MovementSystem() = default;

	void init() {  }

	//updating position
	void update(std::unique_ptr<EntityPlayer>&);
	void update(const std::vector<std::unique_ptr<EntityEnemy>>&);
	void update(const std::vector<std::unique_ptr<EntityBullet>>&);
	
	//updating rotation
	void update_rotation(std::unique_ptr<EntityPlayer>&);
	
	
	//others
	void checkMaxDist_Bullet(const std::vector<std::unique_ptr<EntityBullet>>&);


};