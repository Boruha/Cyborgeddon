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

struct EntityDoor : Entity
{
    EntityDoor() = default;
    explicit EntityDoor(const Device& device, const Vector3f& pos = Vector3f(0,0,0), const Vector3f& dim = Vector3f(5)) :
            Entity(DOOR_ID), transformable(pos), collider(dim, transformable), node(device, pos, dim) { node.setTexture(renderable.texture); }
    ~EntityDoor() override = default;

    Renderable					renderable {"", "./img/textures/testing/testing_cube.png"};
    Transformable 				transformable;
    BoundingBox					collider;

    SceneNode 					node;
    bool                        open = false;
};