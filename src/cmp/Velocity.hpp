#pragma once

#include <SunlightEngine/Vector3.hpp>

using Sun::Vector3f;

struct Velocity
{
/*	Vector3f	velocity;	*/
	Velocity() = default;
	explicit Velocity(const float speed) : speed(speed){ }
	~Velocity() = default;

	Vector3f	 velocity  {0,0,0};
	Vector3f	 direccion {0,0,0};
	const float  speed {0};
};