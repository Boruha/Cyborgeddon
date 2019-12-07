#pragma once

#include <cmp/Camera.hpp>

#include <ent/Entity.hpp>

#include <irrlicht/vector3d.h>
#include <irrlicht/ISceneManager.h>

struct EntityCamera : Entity
{
	explicit EntityCamera(irr::IrrlichtDevice* device) : Entity(CAMERA_ID), camera(device->getSceneManager()->addCameraSceneNode())
	{
		camera.cameraNode->setPosition(irr::core::vector3df(0,60,-40));
	}

	Camera camera;
};