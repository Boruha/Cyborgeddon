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
	EntityPlayer() : Entity(PLAYER_ID) {  }
	explicit EntityPlayer(const Device& device, const Vector3f& pos = Vector3f(), const Vector3f& dim = Vector3f(15), const float speed = 1.f) :
		Entity(PLAYER_ID), transformable(pos), collider(dim), velocity(speed), node(device, pos, dim) { node.setTexture(renderable.texture); }

	Renderable					renderable {"", "./img/textures/testing/testing_player.png"};
	Transformable 				transformable;
	BoundingBox					collider;
	Velocity 					velocity;

	SceneNode 					node;

    std::vector<int>            owned_keys;
	//NEXT CMP SHOOTING(?);
	bool 	                    shooting { false };
	bool 	                    mode { false };		//true = angel, false = demon;
};