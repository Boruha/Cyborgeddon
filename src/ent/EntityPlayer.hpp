#pragma once

#include <cmp/Renderable.hpp>
#include <cmp/Transformable.hpp>
#include <cmp/Velocity.hpp>

#include <ent/Entity.hpp>

#include <irrlicht/irrlicht.h>

struct EntityPlayer : Entity
{
	EntityPlayer() = default;
	explicit EntityPlayer(irr::IrrlichtDevice* device) :
		Entity(PLAYER_ID), node(device->getSceneManager()->addCubeSceneNode(10.f))
	{
		node->setPosition(irr::core::vector3df(0,0,5));
		node->setMaterialTexture(0, device->getVideoDriver()->getTexture(renderable.texture));
		node->setMaterialFlag(irr::video::EMF_LIGHTING, false);
	}
	~EntityPlayer() override = default;

	irr::scene::ISceneNode* 	node { nullptr };
	Renderable					renderable {"", "./img/textures/testing/testing_cube.png"};
	Transformable 				transformable;
	Velocity 					velocity;
};