#pragma once

#include <cmp/Transformable.hpp>
#include <cmp/BoundingBox.hpp>
#include <cmp/Lock.hpp>

#include <ent/Entity.hpp>

#include <SunlightEngine/Vector3.hpp>
#include <SunlightEngine/SceneNode.hpp>

using Sun::Vector3f;
using Sun::SceneNode;

struct EntityDoor : Entity {

	explicit EntityDoor() : Entity(DOOR) {  }

    explicit EntityDoor(Transformable& transformable, BoundingBox& box, Lock& lock, SceneNode& node) : Entity(DOOR),
    	transformable(&transformable), collider(&box), lock(&lock), node(&node)
	{

	}

	~EntityDoor() override {
    	node->removeFromScene();
    	transformable->makeUndefined();
    	collider->makeUndefined();
    	lock->makeUndefined();
    }

    Transformable*		transformable { nullptr };
    BoundingBox*			 collider { nullptr };
    Lock*						 lock { nullptr };
    SceneNode*					 node { nullptr };
};