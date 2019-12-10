#pragma once

#include <cmp/Velocity.hpp>
#include <cmp/Transformable.hpp>

#include <ent/Entity.hpp>

#include <irrlicht/irrlicht.h>

struct EntityDoor : Entity
{
	explicit EntityDoor(irr::IrrlichtDevice* device) : Entity(DOOR_ID), node(device->getSceneManager()->addCubeSceneNode(20.f))
	{
		node->setPosition(irr::core::vector3df(20,0,20));
	}

	irr::scene::ISceneNode* 	node { nullptr };
	Transformable 				transformable;
};