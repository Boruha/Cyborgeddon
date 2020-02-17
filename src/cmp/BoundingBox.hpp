#pragma once

#include <cmp/Component.hpp>

struct BoundingBox : Component
{
	explicit BoundingBox(EntityType e_type, std::size_t e_ID, const vec3& dim, vec3& pos, vec3& vel, bool passable, ColliderType type);
	explicit BoundingBox(EntityType e_type, std::size_t e_ID, const vec3& dim, vec3& pos, bool passable, ColliderType type);

	[[nodiscard]] vec3& operator[](int index);

	[[nodiscard]] const vec3& operator[](int index) const;

	friend std::ostream& operator<<(std::ostream& os, const BoundingBox& box);

	bool 		 passable 	{  false  }; // este bool controla si este collider puede ser atravesado por otros collider o no
	ColliderType type 	 	{ DYNAMIC };

	vec3  dim;
	vec3* pos 		{ nullptr };
	vec3* velocity 	{ nullptr };

	vec3 min {0};
	vec3 max {0};
};