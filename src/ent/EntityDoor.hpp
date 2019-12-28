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

    explicit EntityDoor(const Transformable& transformable, const Vector3f& dim, const Lock& lock, SceneNode& node) : Entity(DOOR_ID),
    	transformable(&transformable), collider(dim, transformable.position), lock(&lock), node(&node)
	{

	}

	~EntityDoor() {
    	std::cout << "Muere una puerta" << std::endl;
    	node->removeFromScene();
    }

    const Transformable*	transformable { nullptr };
    BoundingBox	collider;
    const Lock*						 lock { nullptr };
    SceneNode*						 node { nullptr };

    Alive 						alive;
};