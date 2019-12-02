#pragma once

#include <irrlicht/ICameraSceneNode.h>

struct Camera
{
	explicit Camera(irr::scene::ICameraSceneNode* camera) : cameraNode(camera), angle(0), distance(10) {}
	irr::scene::ICameraSceneNode*	cameraNode;
	float angle;
	float distance;
};