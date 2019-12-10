#pragma once

#include <cmp/Velocity.hpp>
#include <cmp/Transformable.hpp>

#include <ent/Entity.hpp>

#include <irrlicht/irrlicht.h>

struct EntityKey : Entity
{
	explicit EntityKey(irr::IrrlichtDevice* device) : Entity(KEY_ID), node(device->getSceneManager()->addCubeSceneNode(2.f))
	{
		node->setPosition(irr::core::vector3df(0,0,5));
	}

	irr::scene::ISceneNode* 	node { nullptr };
	Transformable 				transformable;
};