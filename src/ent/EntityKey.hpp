#pragma once

#include <cmp/Renderable.hpp>
#include <cmp/Transformable.hpp>
#include <cmp/Velocity.hpp>
#include <cmp/BoundingBox.hpp>

#include <ent/Entity.hpp>

#include <SunlightEngine/Vector3.hpp>
#include <SunlightEngine/SceneNode.hpp>
#include <SunlightEngine/Device.hpp>

using Sun::Vector3f;
using Sun::SceneNode;
using Sun::Device;

struct EntityKey : Entity
{
    explicit EntityKey(const Transformable& transformable, const Vector3f& dim, const Lock& lock, SceneNode& node) :
    	Entity(KEY_ID), transformable(&transformable), collider(dim, transformable.position), lock(&lock), node(&node)
	{

	}

	~EntityKey() {
    	std::cout << "Muere una llave" << std::endl;
    	node->removeFromScene();
    }

    const Transformable* 	transformable { nullptr };
    BoundingBox	collider;
    const Lock*						 lock { nullptr };
    SceneNode* 					  	 node { nullptr };

    Alive						alive;
};