#pragma once

#include <cmp/Transformable.hpp>
#include <cmp/Physics.hpp>
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
	explicit EntityBullet() : Entity(BULLET) {  }

	explicit EntityBullet(Physics& physics, BoundingBox& box, const bool type, SceneNode& node) :
			Entity(BULLET), physics(&physics), collider(&box), node(&node),
			dmgType(type)
	{

	}

	~EntityBullet() override {
		std::cout << "Muere una bala" << std::endl;
		node->removeFromScene();
	}

	Physics*			  physics { nullptr };
	BoundingBox*		 collider { nullptr };
	SceneNode*				 node { nullptr };

    float distance_left { 150 };
	bool dmgType { false };
};