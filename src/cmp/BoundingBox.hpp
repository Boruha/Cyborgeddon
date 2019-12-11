#pragma once

#include <SunlightEngine/Vector3.hpp>

using Sun::Vector3f;

struct BoundingBox
{
	explicit BoundingBox(const Vector3f& dim, const Transformable& trans) : dim(dim), transformable(trans)
	{
		// TODO : BOUNDINGBOX A TRAVÉS DE UNA MALLA DE PUNTOS
	}

	const Vector3f dim {0,0,0};
	Transformable transformable {0,0,0};
};