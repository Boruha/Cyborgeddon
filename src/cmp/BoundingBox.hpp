#pragma once

#include <SunlightEngine/Vector2.hpp>
#include <SunlightEngine/Vector3.hpp>
#include <utility>
#include <iostream>

using Sun::Vector2f;
using Sun::Vector3f;

struct BoundingBox
{
	explicit BoundingBox(const Vector3f& dim)
		: dim(dim), radio(Vector2f(dim.x / 2).length())
	{
		// TODO : BOUNDINGBOX A TRAVÉS DE UNA MALLA DE PUNTOS
	}

	const Vector3f dim {0,0,0};
	// este radio es temporal, para calcular colisiones circulares
	const float radio {0};
	// es la diagonal de un cuadrado de lado dim.x (todos los valores de dim son iguales, no importa que sea .x)
};