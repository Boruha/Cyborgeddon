#pragma once

#include <cmp/Transformable.hpp>
#include <cmp/BoundingBox.hpp>

#include <ent/Entity.hpp>

#include <SunlightEngine/Vector3.hpp>
#include <SunlightEngine/SceneNode.hpp>

using Sun::Vector3f;
using Sun::SceneNode;

struct EntityWall : Entity
{
    explicit EntityWall(const Transformable& transformable, const Vector3f& dim, const SceneNode& node) : Entity(WALL_ID),
    	transformable(&transformable), collider(dim, transformable.position), node(&node)
	{

	}

    const Transformable* 	transformable { nullptr };
    BoundingBox	collider;
    const SceneNode* 				 node { nullptr };
};