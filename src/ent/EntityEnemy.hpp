#pragma once

#include <ent/Entity.hpp>
#include <irrlicht/irrlicht.h>

struct EntityEnemy : Entity
{
	explicit EntityPlayer(irr::IrrlichtDevice* device, irr::core::vector3df& new_pos, float vel) : Entity(device, new_pos, vel){}
	//0 stopped, 1 chasing.
	int 	ai_state { 0 }; 
};