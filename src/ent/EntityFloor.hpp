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

    explicit EntityFloor(const Device& device, const char* tex, const Vector3f& pos, const Vector3f& dim) :
    	Entity(FLOOR_ID), transformable(pos), node(device, pos, dim)
	{
    	renderable.texture = tex;
    	node.setTexture(renderable.texture);
	}

    Transformable 				transformable;
	Renderable 					renderable {"", ""};

    SceneNode 					node;
};