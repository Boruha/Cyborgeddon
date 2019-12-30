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
	void updatePlayerAndCamera(std::unique_ptr<EntityPlayer>&, std::unique_ptr<EntityCamera>&) const;
	
	//others
	void checkMaxDist_Bullet(const std::vector<std::unique_ptr<Entity>>&) const;
};