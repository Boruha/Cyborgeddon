#pragma once

#include <SunlightEngine/Vector3.hpp>

using Sun::Vector3f;

struct Transformable
{
	Transformable() = default;
	explicit Transformable(Vector3f& pos) : position(pos) {}
	Transformable(float x, float y, float z) : position(x,y,z) {}
	~Transformable() = default;

	Vector3f	position {0,0,0};
	Vector3f	rotation {0,0,0};
	Vector3f	scale 	 {1,1,1};
};