#pragma once

#include <SunlightEngine/Vector3.hpp>
#include <cmp/Component.hpp>

using Sun::Vector3f;

struct BoundingBox : Component
{
	BoundingBox() : Component(UNDEFINED, 0) { };
	explicit BoundingBox(const EntityType e_type, const std::size_t e_ID, const Vector3f& dim, const Vector3f& pos, Vector3f& vel, const bool passable, const ColliderType type)
		: Component(e_type, e_ID), dim(dim), pos(&pos), velocity(&vel), passable(passable), type(type)
	{
		/* TODO : BOUNDINGBOX A TRAVÉS DE UNA MALLA DE PUNTOS */
	}

	[[nodiscard]] Vector3f& operator[](int index);

	[[nodiscard]] const Vector3f& operator[](int index) const;

	const Vector3f dim;
	const Vector3f* pos { nullptr };
	Vector3f* velocity 	{ nullptr };

	Vector3f min {0};
	Vector3f max {0};

	const bool passable { false }; // este bool controla si este collider puede ser atravesado por otros collider o no

	ColliderType type { ColliderType::DYNAMIC };
};