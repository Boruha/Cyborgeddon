#pragma once

#include <cmp/Camera.hpp>

#include <ent/Entity.hpp>

#include <irrlicht/ISceneManager.h>

struct EntityCamera : Entity
{
	explicit EntityCamera(irr::IrrlichtDevice* device, const Vector3f& target) :
		Entity(CAMERA_ID),
		camera(device->getSceneManager()->addCameraSceneNode(), target, Vector3f(0,20,-30)) { }
	~EntityCamera() override = default;

	Camera camera;
	Velocity velocity;
};