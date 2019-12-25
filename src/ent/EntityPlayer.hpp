#pragma once

#include <cmp/Renderable.hpp>
#include <cmp/Transformable.hpp>
#include <cmp/BoundingBox.hpp>
#include <cmp/Velocity.hpp>

#include <ent/Entity.hpp>

#include <SunlightEngine/Vector3.hpp>
#include <SunlightEngine/SceneNode.hpp>
#include <SunlightEngine/Device.hpp>

#include <vector>

using Sun::Vector3f;
using Sun::SceneNode;
using Sun::Device;

struct EntityPlayer : Entity
{
	explicit EntityPlayer(const Device& device, const int hp, const Vector3f& pos, const Vector3f& dim,
			const float speed = 1.f) : Entity(PLAYER_ID), transformable(pos), collider(dim, pos), velocity(speed),
			node(device, pos, dim), health(hp)
	{
		node.setTexture(renderable.texture);
	}

	Renderable					renderable {"", "./img/textures/testing/testing_demon.jpg"};
	Transformable 				transformable;
	BoundingBox					collider;
	Velocity 					velocity;

	SceneNode 					node;

    std::vector<bool>           my_keys;
	//NEXT CMP SHOOTING(?);
	bool 	                    shooting { false };
	bool 	                    mode { false };		// true(1) = angel, false(0) = demon;
	int                         health { 0 };
};