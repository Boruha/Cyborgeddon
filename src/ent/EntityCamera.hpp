#pragma once

#include <cmp/Camera.hpp>

#include <ent/Entity.hpp>

#include <SunlightEngine/Device.hpp>
#include <SunlightEngine/Vector3.hpp>

using Sun::Device;
using Sun::Vector3f;

struct EntityCamera : Entity
{
	explicit EntityCamera(const Device& device, const Vector3f& pos = Vector3f(0, 20, -30), const Vector3f& target = Vector3f())
		: Entity(CAMERA_ID), transformable(pos), camera(target), cameraNode(device, pos, target) {  }
	~EntityCamera() override = default;

	Transformable transformable;
	Velocity velocity;
	Camera camera;

	CameraNode cameraNode;
};