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
    explicit EntityKey(const Device& device, const Vector3f& pos = Vector3f(), const Vector3f& dim = Vector3f(5), const int& type = 0) :
            Entity(KEY_ID), transformable(pos), collider(dim), node(device, pos, dim), type(type) { node.setTexture(renderable.texture); }

    Renderable					renderable {"", "./img/textures/testing/testing_key.png"};
    Transformable 				transformable;
    BoundingBox					collider;

    SceneNode 					node;
    bool                        taken { false };
    int                         type;
};