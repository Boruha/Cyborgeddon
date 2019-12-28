#pragma once

#include <cmp/Renderable.hpp>
#include <cmp/Transformable.hpp>

#include <ent/Entity.hpp>

#include <SunlightEngine/Vector3.hpp>
#include <SunlightEngine/SceneNode.hpp>
#include <SunlightEngine/Device.hpp>

using Sun::Vector3f;
using Sun::SceneNode;
using Sun::Device;

struct EntityFloor : Entity {

    explicit EntityFloor(const char* tex, const Transformable& transformable, const SceneNode& node) :
    	Entity(FLOOR_ID), transformable(&transformable), node(&node)
	{

	}

    const Transformable* 		transformable { nullptr };
	const SceneNode*					 node { nullptr };
};