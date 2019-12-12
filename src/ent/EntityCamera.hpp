#pragma once

#include <cmp/Camera.hpp>
#include <cmp/Transformable.hpp>
#include <cmp/Velocity.hpp>

#include <ent/Entity.hpp>

#include <SunlightEngine/Device.hpp>
#include <SunlightEngine/Vector3.hpp>

using Sun::Device;
using Sun::Vector3f;

struct EntityCamera : Entity
{
	explicit EntityCamera(const Device& device, const Vector3f& pos = Vector3f(0, 20, -30), const Vector3f& target = Vector3f(), const float speed = 1.f)
		: Entity(CAMERA_ID), transformable(pos), camera(target), cameraNode(device, pos, target), velocity(speed) {  }

	Transformable transformable;
	Camera camera;
	CameraNode cameraNode;
	Velocity velocity;

};