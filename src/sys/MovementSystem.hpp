#pragma once

#include <ent/Entities.hpp>

#include <src/man/EntityManager.hpp>
#include <sys/System.hpp>

struct MovementSystem : System
{
	void init() override {  }

	void update(const std::unique_ptr<GameContext>& context) const override;

private:
	//updating position
	void updatePlayerAndCamera(std::unique_ptr<EntityPlayer>&, EntityCamera&) const;
	void updateEnemies(const std::vector<std::unique_ptr<EntityEnemy>>&) const;
	void updateBullets(const std::vector<std::unique_ptr<EntityBullet>>&) const;
	
	//others
	void checkMaxDist_Bullet(const std::vector<std::unique_ptr<EntityBullet>>&) const;
};