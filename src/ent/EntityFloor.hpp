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

struct EntityFloor : Entity{

    explicit EntityFloor(const Device& device, const char* tex, const Vector3f& pos = Vector3f(0,0,0), const Vector3f& dim = Vector3f(5)) :
            Entity(DOOR_ID), transformable(pos), node(device, pos, dim) { node.setTexture(tex);  }

    Transformable 				transformable;

    SceneNode 					node;

};