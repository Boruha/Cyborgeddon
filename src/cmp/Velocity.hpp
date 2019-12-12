#pragma once

#include <SunlightEngine/Vector3.hpp>

using Sun::Vector3f;

struct Velocity
{
	Velocity() = default;
	explicit Velocity(const float speed) : speed(speed){ }

	Vector3f	 velocity  {0,0,0};
	Vector3f	 direction {0, 0, 0};
	float  speed {0};
};