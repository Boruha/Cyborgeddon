#pragma once

#include <cmp/Renderable.hpp>
#include <cmp/Transformable.hpp>
#include <cmp/Velocity.hpp>
#include <cmp/BoundingBox.hpp>

#include <ent/Entity.hpp>

#include <irrlicht/irrlicht.h>
#include <iostream>

using Sun::Vector3f;

struct EntityPlayer : Entity
{
	EntityPlayer() = default;
	explicit EntityPlayer(irr::IrrlichtDevice* device, const Vector3f& pos = Vector3f(0,0,0), const Vector3f& dim = Vector3f(10), const float& speed = 1) :
		Entity(PLAYER_ID), transformable(pos), collider(dim, transformable), velocity(speed), node(device->getSceneManager()->addCubeSceneNode(dim.x)) // tomaremos dim.x como referencia temporal
	{
		node->setPosition(irr::core::vector3df(pos.x,pos.y,pos.z));
		node->setMaterialTexture(0, device->getVideoDriver()->getTexture(renderable.texture));
		node->setMaterialFlag(irr::video::EMF_LIGHTING, false);
	}
	~EntityPlayer() override { std::cout << "Destruyendo nodo\n"; }

	Renderable					renderable {"", "./img/textures/testing/testing_cube.png"};
	Transformable 				transformable;
	BoundingBox					collider;
	Velocity 					velocity;

	irr::scene::ISceneNode* 	node { nullptr };
};