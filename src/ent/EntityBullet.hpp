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
	explicit EntityBullet(const Device& device, const Vector3f& pos, const Vector3f& dim, const Vector3f& dir,
			const bool type, const float& speed = 5) :
			Entity(BULLET_ID), transformable(pos), collider(dim, pos), velocity(dir, speed), node(device, pos, dim),
			start_pos(pos), dmgType(type)
	{
		if(dmgType)
			renderable.texture = "./img/textures/testing/testing_angel.jpg";
		else
			renderable.texture = "./img/textures/testing/testing_demon.jpg";

		node.setTexture(renderable.texture);

		velocity.velocity = velocity.direction.normalize() * velocity.speed;
	}

	Renderable					renderable {"", ""};
	Transformable 				transformable;
	BoundingBox					collider;
	Velocity 					velocity;	// TODO: la bala no cambia su vector velocity desde que se crea

	SceneNode 					node;

    //TODO: CREAR CMP DEAD OR SMTH.
	bool alive { true };
    const Vector3f start_pos;
    const float dead_dist { 150 };
	const bool dmgType;
};