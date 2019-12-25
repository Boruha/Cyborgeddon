#pragma once

#include <cmp/Camera.hpp>
#include <cmp/Transformable.hpp>
#include <cmp/Velocity.hpp>

#include <ent/Entity.hpp>

#include <SunlightEngine/Device.hpp>
#include <SunlightEngine/CameraNode.hpp>
#include <SunlightEngine/Vector3.hpp>

using Sun::Device;
using Sun::Vector3f;
using Sun::CameraNode;

struct EntityCamera : Entity
{
	explicit EntityCamera(const Device& device, const Vector3f& pos, const Vector3f& target, const float speed = 1.f)
		: Entity(CAMERA_ID), transformable(pos), camera(target), velocity(speed), cameraNode(device, target, pos)
	{

	}

	Transformable transformable;
	Camera camera;
	Velocity velocity;

	CameraNode cameraNode;
};