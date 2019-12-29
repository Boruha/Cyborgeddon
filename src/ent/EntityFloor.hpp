#pragma once

#include <cmp/Transformable.hpp>

#include <ent/Entity.hpp>

#include <SunlightEngine/Vector3.hpp>
#include <SunlightEngine/SceneNode.hpp>

using Sun::Vector3f;
using Sun::SceneNode;

struct EntityFloor : Entity {

    explicit EntityFloor(const Transformable& transformable, const SceneNode& node) :
    	Entity(FLOOR_ID), transformable(&transformable), node(&node)
	{

	}

    const Transformable* 		transformable { nullptr };
	const SceneNode*					 node { nullptr };
};