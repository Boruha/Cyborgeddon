#include <sys/MovementSystem.hpp>
#include <iostream>

void MovementSystem::update(EntityPlayer& player)
{
	irr::core::vector3df new_pos = player.node->getPosition();

	new_pos.X += (player.vDirection.X * player.maxVel);	
	new_pos.Z += (player.vDirection.Z * player.maxVel);	

	player.node->setPosition(new_pos);
}

void MovementSystem::update(EntityEnemy& enemy)
{
	irr::core::vector3df new_pos = enemy.node->getPosition();

	new_pos.X += (enemy.vDirection.X * enemy.maxVel);
	new_pos.Z += (enemy.vDirection.Z * enemy.maxVel);	

	enemy.node->setPosition(new_pos);
}