#pragma once

#include <cmp/Transformable.hpp>
#include <cmp/Velocity.hpp>
#include <cmp/BoundingBox.hpp>
#include <cmp/Alive.hpp>

#include <ent/Entity.hpp>

#include <SunlightEngine/Vector3.hpp>
#include <SunlightEngine/SceneNode.hpp>

using Sun::Vector3f;
using Sun::SceneNode;

struct EntityBullet : Entity
{
	explicit EntityBullet(Transformable& transformable, Velocity& velocity, const Vector3f& dim, const bool type, SceneNode& node) :
			Entity(BULLET_ID), transformable(&transformable), velocity(&velocity), collider(dim, transformable.position), node(&node),
			dmgType(type)
	{

	}

	~EntityBullet() {
		std::cout << "Muere una bala" << std::endl;
		node->removeFromScene();
	}

	Transformable*	transformable { nullptr };
	Velocity*			 velocity { nullptr };
	BoundingBox			 collider;
	SceneNode*				 node { nullptr };


    //TODO: CREAR CMP DEAD OR SMTH.
	Alive alive;

    float distance_left { 150 };
	const bool dmgType;
};