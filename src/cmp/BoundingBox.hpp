#pragma once

#include <SunlightEngine/Vector3.hpp>
#include <cmp/Component.hpp>

using Sun::Vector3f;

struct BoundingBox : Component
{
	explicit BoundingBox(EntityType e_type, std::size_t e_ID, const Vector3f& dim, const Vector3f& pos, Vector3f& vel, bool passable, ColliderType type);

	[[nodiscard]] Vector3f& operator[](int index);

	[[nodiscard]] const Vector3f& operator[](int index) const;

	friend std::ostream& operator<<(std::ostream& os, const BoundingBox& box);

	bool 		 passable 	{  false  }; // este bool controla si este collider puede ser atravesado por otros collider o no
	ColliderType type 	 	{ DYNAMIC };

	const	Vector3f  		 	dim;
	const 	Vector3f * const 	pos 		{ nullptr };
		  	Vector3f * const 	velocity 	{ nullptr };

	Vector3f min {0};
	Vector3f max {0};
};