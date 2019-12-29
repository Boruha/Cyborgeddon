#pragma once

#include <SunlightEngine/Vector3.hpp>

using Sun::Vector3f;

struct AABB {
	AABB (const Vector3f& dimension, const Vector3f& position);

	void setBox(const Vector3f& dimension, const Vector3f& position);
	void moveCoord(const float& mov, const int& coord);

	[[nodiscard]] bool intersects(const AABB& box) const;

	Vector3f min {0}, max {0};
};

struct BoundingBox
{
	explicit BoundingBox(const Vector3f& dim, const Vector3f& pos)
		: dim(dim), pos(&pos), box(dim, pos) { /* TODO : BOUNDINGBOX A TRAVÉS DE UNA MALLA DE PUNTOS */ }

	void fixBox();

	void moveCoord(const float& mov, const int& coord);

	[[nodiscard]] bool intersects(const BoundingBox& other) const;

	const Vector3f dim;
	const Vector3f* pos { nullptr };
	AABB box;
};