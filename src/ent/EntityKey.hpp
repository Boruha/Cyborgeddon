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
	explicit EntityKey() : Entity(KEY) {  };

    explicit EntityKey(Transformable& transformable, BoundingBox& box, Lock& lock, SceneNode& node) :
			Entity(KEY), transformable(&transformable), collider(&box), lock(&lock), node(&node)
	{

	}

	~EntityKey() override {
    	std::cout << "Muere una llave" << std::endl;
    	node->removeFromScene();
    }

    Transformable* 	transformable { nullptr };
    BoundingBox*		 collider { nullptr };
    Lock*					 lock { nullptr };
    SceneNode* 			  	 node { nullptr };
};