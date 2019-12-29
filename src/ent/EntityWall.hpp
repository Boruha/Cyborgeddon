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
    explicit EntityWall(const Transformable& transformable, const BoundingBox& box, const SceneNode& node) : Entity(WALL_ID),
    	transformable(&transformable), collider(&box), node(&node)
	{

	}

    const Transformable* 	transformable { nullptr };
    const BoundingBox*			 collider { nullptr };
    const SceneNode* 				 node { nullptr };
};