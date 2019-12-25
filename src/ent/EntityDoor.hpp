#pragma once

#include <cmp/Renderable.hpp>
#include <cmp/Transformable.hpp>
#include <cmp/Velocity.hpp>
#include <cmp/BoundingBox.hpp>
#include <cmp/Lock.hpp>

#include <ent/Entity.hpp>

#include <SunlightEngine/Vector3.hpp>
#include <SunlightEngine/SceneNode.hpp>
#include <SunlightEngine/Device.hpp>

using Sun::Vector3f;
using Sun::SceneNode;
using Sun::Device;

struct EntityDoor : Entity {

    explicit EntityDoor(const Device& device, const Vector3f& pos, const Vector3f& dim) : Entity(DOOR_ID),
    	transformable(pos), collider(dim, pos), node(device, pos, dim)
	{
    	node.setTexture(renderable.texture);
	}

    Renderable					renderable {"", "./img/textures/testing/testing_door.png"};
    Transformable 				transformable;
    BoundingBox					collider;

    SceneNode 					node;

    const Lock                  lock;
    bool 						alive { true };
};