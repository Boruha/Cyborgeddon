#pragma once

#include <SunlightEngine/Vector3.hpp>

using Sun::Vector3f;

struct Velocity
{
	explicit Velocity(const float speed) : speed(speed){ }
	explicit Velocity(const Vector3f& dir, const float speed) : direccion(dir), speed(speed){ }

	Vector3f	 velocity  {0,0,0};
	Vector3f	 direccion {0,0,0};
	const float  speed {0};
};