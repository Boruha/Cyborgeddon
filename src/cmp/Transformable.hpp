#pragma once

#include <SunlightEngine/Vector3.hpp>

using Sun::Vector3f;

struct Transformable
{
	Transformable() = default;
	explicit Transformable(const Vector3f& pos) : position(pos) {}
	explicit Transformable(const float x, const float y, const float z) : position(x,y,z) {}

	Vector3f	position {0,0,0};
	Vector3f	rotation {0,0,0};
	Vector3f	scale 	 {1,1,1};
};