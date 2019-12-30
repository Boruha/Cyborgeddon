#pragma once

#include <cmp/Transformable.hpp>

#include <ent/Entity.hpp>

#include <SunlightEngine/Vector3.hpp>
#include <SunlightEngine/SceneNode.hpp>

using Sun::Vector3f;
using Sun::SceneNode;

struct EntityFloor : Entity {

	explicit EntityFloor() : Entity(FLOOR) {  };

    explicit EntityFloor(Transformable& transformable, SceneNode& node) :
			Entity(FLOOR), transformable(&transformable), node(&node)
	{

	}

    Transformable* 		transformable { nullptr };
    SceneNode*					 node { nullptr };
};