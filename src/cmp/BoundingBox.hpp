#pragma once

#include <cmp/Component.hpp>

struct BoundingBox : Component
{
	explicit BoundingBox(EntityType e_type, std::size_t e_ID, const Vector3f& dim, Vector3f& pos, Vector3f& vel, bool passable, ColliderType type);
	explicit BoundingBox(EntityType e_type, std::size_t e_ID, const Vector3f& dim, Vector3f& pos, bool passable, ColliderType type);

	[[nodiscard]] Vector3f& operator[](int index);

	[[nodiscard]] const Vector3f& operator[](int index) const;

	friend std::ostream& operator<<(std::ostream& os, const BoundingBox& box);

	bool 		 passable 	{  false  }; // este bool controla si este collider puede ser atravesado por otros collider o no
	ColliderType type 	 	{ DYNAMIC };

	Vector3f  dim;
	Vector3f* pos 		{ nullptr };
	Vector3f* velocity 	{ nullptr };

	Vector3f min {0};
	Vector3f max {0};
};