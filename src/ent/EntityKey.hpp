#pragma once

#include <ent/Entity.hpp>

#include <SunlightEngine/SceneNode.hpp>

using Sun::SceneNode;

struct EntityKey : Entity
{
	explicit EntityKey() : Entity(KEY) {  };

    explicit EntityKey(Transformable& transformable, BoundingBox& box, Lock& lock, SceneNode& node) :
			Entity(KEY), transformable(&transformable), collider(&box), lock(&lock), node(&node)
	{

	}

	~EntityKey() override {
    	node->removeFromScene();

    	transformable->makeUndefined();
    	collider->makeUndefined();
    	// lock no pertenece a la llave sino a la puerta, asi que no hay que hacerlo undefined
    }

    Transformable* 	transformable { nullptr };
    BoundingBox*		 collider { nullptr };
    Lock*					 lock { nullptr };
    SceneNode* 			  	 node { nullptr };
};