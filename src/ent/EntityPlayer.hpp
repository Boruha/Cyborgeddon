#pragma once

#include <cmp/Physics.hpp>
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
	explicit EntityPlayer() : Entity(PLAYER) {  }

	explicit EntityPlayer(Physics& physics, Velocity& velocity, BoundingBox& box, SceneNode& node) :
			Entity(PLAYER), physics(&physics), velocity(&velocity), collider(&box), node(&node) {  }

	Physics*	  physics { nullptr };
	Velocity* 	 velocity { nullptr };
	BoundingBox* collider { nullptr };
	SceneNode* 		 node { nullptr };

	//NEXT CMP SHOOTING(?);
	bool shooting { false };
	bool mode 	  { false };		// true(1) = angel, false(0) = demon;
	int  health   {  100  };
};