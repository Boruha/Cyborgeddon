#pragma once

#include <cmp/Transformable.hpp>
#include <cmp/BoundingBox.hpp>

#include <ent/Entity.hpp>

#include <SunlightEngine/SceneNode.hpp>

using Sun::SceneNode;

struct EntityWall : Entity
{
	explicit EntityWall() : Entity(WALL) {  }

    explicit EntityWall(Transformable& transformable, BoundingBox& box, SceneNode& node) : Entity(WALL),
    	transformable(&transformable), collider(&box), node(&node)
	{

	}

	~EntityWall() override {
		node->removeFromScene();
		transformable->makeUndefined();
		collider->makeUndefined();
	}

    Transformable* transformable { nullptr };
    BoundingBox*		collider { nullptr };
    SceneNode* 				node { nullptr };
};