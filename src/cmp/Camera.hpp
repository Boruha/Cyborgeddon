#pragma once

#include <SunlightEngine/Vector3.hpp>
#include <SunlightEngine/Device.hpp>
#include <SunlightEngine/CameraNode.hpp>

using Sun::Vector3f;
using Sun::Device;
using Sun::CameraNode;

struct Camera
{
	explicit Camera(const Vector3f& target = Vector3f(), const float angle = 0.f, const float distance = 10.f)
		: target(target), angle(angle), distance(distance) {  }

	Vector3f target {0,0,0};
	float angle {0};
	float distance {10};
};