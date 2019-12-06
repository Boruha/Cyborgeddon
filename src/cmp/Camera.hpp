#pragma once

#include <irrlicht/ICameraSceneNode.h>
#include <SunlightEngine/Vector3.hpp>
#include <iostream>

using Sun::Vector3f;

struct Camera
{
	explicit Camera(irr::scene::ICameraSceneNode* camera, const Vector3f& target, const Vector3f& pos) :
		cameraNode(camera),
		target(target)
		{
			cameraNode->setPosition(irr::core::vector3df(pos.x, pos.y, pos.z));
			cameraNode->setTarget(irr::core::vector3df(target.x, target.y, target.z));
		}
	~Camera() = default;

	irr::scene::ICameraSceneNode* cameraNode { nullptr };
	float angle {0};
	float distance {10};
	const Vector3f target {0,0,0};
};