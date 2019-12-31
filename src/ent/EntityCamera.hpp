#pragma once

#include <cmp/Transformable.hpp>
#include <cmp/Velocity.hpp>

#include <ent/Entity.hpp>

#include <SunlightEngine/Vector3.hpp>
#include <SunlightEngine/CameraNode.hpp>

using Sun::Vector3f;
using Sun::CameraNode;

struct EntityCamera : Entity
{
	explicit EntityCamera() : Entity(CAMERA) {  };

	explicit EntityCamera(Physics& physics, CameraNode& node)
		: Entity(CAMERA), physics(&physics), node(&node)
	{
	}

	~EntityCamera() override {
	//	node->removeFromScene();				muere depsues del device de irrlicht asi que da segmentation fault
		physics->makeUndefined();
	}

	Physics* physics { nullptr };
	CameraNode* node { nullptr };
};