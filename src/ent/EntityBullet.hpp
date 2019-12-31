#pragma once

#include <cmp/Physics.hpp>
#include <cmp/BoundingBox.hpp>
#include <cmp/BulletData.hpp>

#include <ent/Entity.hpp>

#include <SunlightEngine/Vector3.hpp>
#include <SunlightEngine/SceneNode.hpp>

using Sun::Vector3f;
using Sun::SceneNode;

struct EntityBullet : Entity
{
	explicit EntityBullet() : Entity(BULLET) {  }

	explicit EntityBullet(Physics& physics, BoundingBox& box, BulletData& data, SceneNode& node) :
			Entity(BULLET), physics(&physics), collider(&box), data(&data), node(&node)
	{

	}

	~EntityBullet() override {
		node->removeFromScene();
		physics->makeUndefined();
		collider->makeUndefined();
		data->makeUndefined();
	}

	Physics*			  physics { nullptr };
	BoundingBox*		 collider { nullptr };
	BulletData*				 data { nullptr };
	SceneNode*				 node { nullptr };
};