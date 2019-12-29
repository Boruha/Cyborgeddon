#pragma once

#include <cmp/Transformable.hpp>
#include <cmp/Velocity.hpp>
#include <cmp/BoundingBox.hpp>
#include <cmp/Lock.hpp>

#include <ent/Entity.hpp>

#include <SunlightEngine/Vector3.hpp>
#include <SunlightEngine/SceneNode.hpp>

using Sun::Vector3f;
using Sun::SceneNode;

struct EntityDoor : Entity {

    explicit EntityDoor(const Transformable& transformable, const BoundingBox& box, const Lock& lock, SceneNode& node) : Entity(DOOR_ID),
    	transformable(&transformable), collider(&box), lock(&lock), node(&node)
	{

	}

	~EntityDoor() {
    	std::cout << "Muere una puerta" << std::endl;
    	node->removeFromScene();
    }

    const Transformable*	transformable { nullptr };
    const BoundingBox*			 collider { nullptr };
    const Lock*						 lock { nullptr };
    SceneNode*						 node { nullptr };

    Alive 						alive;
};