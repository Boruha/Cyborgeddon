#pragma once

#include <SunlightEngine/Vector3.hpp>

struct BoundingBox
{
	BoundingBox() = default;
	explicit BoundingBox(const Vector3f& dim, const Vector3f& pos) : dimension(dim), position(pos) {  }
	~BoundingBox() = default;

	const Vector3f dimension {0,0,0};
	Vector3f position {0,0,0};
};