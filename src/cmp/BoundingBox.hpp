#pragma once

#include <SunlightEngine/Vector3.hpp>
#include <cmp/Component.hpp>

using Sun::Vector3f;

struct BoundingBox : Component
{
	BoundingBox() : Component(UNDEFINED, 0) { };
	explicit BoundingBox(const EntityType e_type, const std::size_t e_ID, const Vector3f& dim, Vector3f& pos, Vector3f& vel, const bool passable, const ColliderType type)
		: Component(e_type, e_ID), passable(passable), type(type), dim(dim), pos(&pos), velocity(&vel)
	{
		/* TODO : BOUNDINGBOX A TRAVÉS DE UNA MALLA DE PUNTOS */
	}

	[[nodiscard]] Vector3f& operator[](int index);

	[[nodiscard]] const Vector3f& operator[](int index) const;

	friend std::ostream& operator<<(std::ostream& os, const BoundingBox& box);

	bool passable { false }; // este bool controla si este collider puede ser atravesado por otros collider o no
	ColliderType type { ColliderType::DYNAMIC };

	Vector3f dim;
	Vector3f* pos { nullptr };
	Vector3f* velocity 	{ nullptr };

	Vector3f min {0};
	Vector3f max {0};

};