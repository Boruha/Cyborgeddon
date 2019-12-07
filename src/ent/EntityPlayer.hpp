#pragma once

//#include <cmp/Velocity.hpp>
//#include <cmp/Transformable.hpp>

#include <ent/Entity.hpp>

#include <irrlicht/irrlicht.h>

struct EntityPlayer : Entity
{
	explicit EntityPlayer(irr::IrrlichtDevice* device) : Entity(PLAYER_ID), node(device->getSceneManager()->addCubeSceneNode(10.f))
	{
		node->setPosition(irr::core::vector3df(0,0,5));
	}
	
	/* FUTURE COMPONENTS */
	
	//Body of the entity, includes (will change) vectors position, rotation and scale.
	irr::scene::ISceneNode* 	node { nullptr };
    //normaliced vector which indicates the direction of our movement.
	irr::core::vector3df 		vDirection = irr::core::vector3df(0.f);
	//max velocity in 1 axis.
    const float 				maxVel { 0.5f };
};