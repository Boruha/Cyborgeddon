#pragma once

#include <cmp/Transformable.hpp>
#include <cmp/Velocity.hpp>
#include <cmp/BoundingBox.hpp>

#include <ent/Entity.hpp>

#include <SunlightEngine/Vector3.hpp>
#include <SunlightEngine/SceneNode.hpp>

using Sun::Vector3f;
using Sun::SceneNode;

struct EntityKey : Entity
{
    explicit EntityKey(const Transformable& transformable, BoundingBox& box, const Lock& lock, SceneNode& node) :
    	Entity(KEY_ID), transformable(&transformable), collider(&box), lock(&lock), node(&node)
	{

	}

	~EntityKey() {
    	std::cout << "Muere una llave" << std::endl;
    	node->removeFromScene();
    }

    const Transformable* 	transformable { nullptr };
    BoundingBox*				 collider { nullptr };
    const Lock*						 lock { nullptr };
    SceneNode* 					  	 node { nullptr };

    Alive						alive;
};