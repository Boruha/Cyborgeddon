#pragma once

#include <cmp/Renderable.hpp>
#include <cmp/Transformable.hpp>
#include <cmp/BoundingBox.hpp>
#include <cmp/Velocity.hpp>

#include <ent/Entity.hpp>

#include <SunlightEngine/Vector3.hpp>
#include <SunlightEngine/SceneNode.hpp>
#include <SunlightEngine/Device.hpp>

using Sun::Vector3f;
using Sun::SceneNode;
using Sun::Device;

struct EntityBullet : Entity
{
	explicit EntityBullet(const Device& device, const Vector3f& pos = Vector3f(0,0,0), const Vector3f& dim = Vector3f(4),
			const Vector3f& dir = Vector3f(0,0,1), const bool type = false, const float& speed = 15) :
		Entity(BULLET_ID), transformable(pos), collider(dim), velocity(dir, speed), node(device, pos, dim), start_pos(pos), dmgType(type)
		{ node.setTexture(renderable.texture); }

	Renderable					renderable {"", "./img/textures/testing/testing_bullet.png"};
	Transformable 				transformable;
	BoundingBox					collider;
	Velocity 					velocity;	// TODO: la bala no cambia su vector velocity desde que se crea

	SceneNode 					node;

    //TODO: CREAR CMP DEAD OR SMTH.
	bool dead { false };
    const Vector3f start_pos { Vector3f() };
    const float dead_dist { 150 };
	const bool dmgType { false };
};