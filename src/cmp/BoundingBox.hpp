#pragma once

#include <SunlightEngine/Vector2.hpp>
#include <SunlightEngine/Vector3.hpp>
#include <utility>

using Sun::Vector2f;
using Sun::Vector3f;

struct AABB {
	AABB () = default;

	explicit AABB (const Vector3f& dimension, const Vector3f& position) {
		setBox(dimension, position);
	}

	void setBox(const Vector3f& dimension, const Vector3f& position) {
		max.x = position.x + dimension.x / 2;
		min.x = position.x - dimension.x / 2;

		max.y = position.y + dimension.y / 2;
		min.y = position.y - dimension.y / 2;

		max.z = position.z + dimension.z / 2;
		min.z = position.z - dimension.z / 2;
	}

	void move(const Vector3f& mov) { max.x += mov.x; min.x += mov.x; max.y += mov.y; min.y += mov.y; max.z += mov.z; min.z += mov.z; }

	[[nodiscard]] bool intersects(const AABB& box) const { return max.x > box.min.x && min.x < box.max.x && max.y > box.min.y && min.y < box.max.y && max.z > box.min.z && min.z < box.max.z; }

	Vector3f min {0}, max {0};
};

struct BoundingBox
{
	explicit BoundingBox(const Vector3f& dim, const Vector3f& pos)
		: dim(dim), pos(pos), box(dim, pos)
	{
		// TODO : BOUNDINGBOX A TRAVÉS DE UNA MALLA DE PUNTOS
	}

	void setPosition(const Vector3f& position) { pos = position; box.setBox(dim, pos); }

	void move(const Vector3f& mov) { pos += mov; box.move(mov); }

	bool intersects(const BoundingBox& other) { return box.intersects(other.box); }

	Vector3f dim { };
	Vector3f pos { };
	AABB box { };
};