#pragma once

#include <cmp/Velocity.hpp>
#include <cmp/Transformable.hpp>

#include <ent/Entity.hpp>

#include <irrlicht/irrlicht.h>

struct EntityPlayer : Entity
{
	explicit EntityPlayer(irr::IrrlichtDevice* device) : Entity(PLAYER_ID), node(device->getSceneManager()->addCubeSceneNode(10.f))
	{
		node->setPosition(irr::core::vector3df(0,0,5));
	}

	irr::scene::ISceneNode* 	node { nullptr };
	Transformable 				transformable;
	Velocity 					velocity;
};