#pragma once

#include <SunlightEngine/Vector3.hpp>
#include <iostream>
#include <cmath>

using Sun::Vector3f;

struct BoundingBox
{
	BoundingBox() = default;
	explicit BoundingBox(const Vector3f& dim, const Transformable& trans) : dim(dim), transformable(trans)
	{
		// TODO : BOUNDINGBOX A TRAVÉS DE UNA MALLA DE PUNTOS
	}
	~BoundingBox() = default;

	const Vector3f dim {0,0,0};
	Transformable transformable {0, 0, 0};
};