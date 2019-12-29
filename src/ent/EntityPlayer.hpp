#pragma once

#include <cmp/Transformable.hpp>
#include <cmp/Velocity.hpp>
#include <cmp/BoundingBox.hpp>
#include <cmp/Alive.hpp>

#include <ent/Entity.hpp>

#include <SunlightEngine/Vector3.hpp>
#include <SunlightEngine/SceneNode.hpp>

#include <vector>

using Sun::Vector3f;
using Sun::SceneNode;

struct EntityPlayer : Entity
{
	explicit EntityPlayer(Transformable& transformable, Velocity& velocity, BoundingBox& box, const SceneNode& node) :
		Entity(PLAYER_ID), transformable(&transformable), velocity(&velocity), collider(&box),
			node(&node)
	{

	}

	Transformable* 	transformable { nullptr };
	Velocity* 			 velocity { nullptr };
	BoundingBox*		 collider { nullptr };
	const SceneNode* 		 node { nullptr };

	Alive						alive;

    std::vector<bool>           my_keys;
	//NEXT CMP SHOOTING(?);
	bool shooting { false };
	bool mode 	  { false };		// true(1) = angel, false(0) = demon;
	int  health   {  100  };
};