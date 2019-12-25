#pragma once

#include <SunlightEngine/Vector3.hpp>
#include <SunlightEngine/Device.hpp>

using Sun::Vector3f;
using Sun::Device;

struct Camera
{
	explicit Camera(const Vector3f& target) : target(&target) {  }

	const Vector3f* target;
};