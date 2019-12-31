#pragma once

#include <cmp/Physics.hpp>
#include <cmp/Velocity.hpp>
#include <cmp/BoundingBox.hpp>

#include <ent/Entity.hpp>

#include <SunlightEngine/SceneNode.hpp>

using Sun::SceneNode;

struct EntityPlayer : Entity
{
	explicit EntityPlayer() : Entity(PLAYER) {  }

	explicit EntityPlayer(Physics& physics, Velocity& velocity, BoundingBox& box, SceneNode& node) :
			Entity(PLAYER), physics(&physics), velocity(&velocity), collider(&box), node(&node) {  }

	~EntityPlayer() override {
	//	node->removeFromScene();	// se destruye despues del device de irrlicht asi que hacer remove da segmentation fault
		physics->makeUndefined();
		velocity->makeUndefined();
		collider->makeUndefined();
	}

	Physics*	  physics { nullptr };
	Velocity* 	 velocity { nullptr };
	BoundingBox* collider { nullptr };
	SceneNode* 		 node { nullptr };

	//NEXT CMP SHOOTING(?);
	bool shooting { false };
	bool mode 	  { false };		// true(1) = angel, false(0) = demon;
	int  health   {  100  };
};