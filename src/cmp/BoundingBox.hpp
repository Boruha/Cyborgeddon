#pragma once

#include <SunlightEngine/Vector3.hpp>
#include <cmp/Component.hpp>

using Sun::Vector3f;

struct BoundingBox : public Component
{
	explicit BoundingBox(const EntityType& e_type, const std::size_t& e_ID, const Vector3f& dim, const Vector3f& pos, const bool is_static)
		: Component(e_type, e_ID), dim(dim), pos(&pos), is_static(is_static)
	{
		/* TODO : BOUNDINGBOX A TRAVÉS DE UNA MALLA DE PUNTOS */
	}

	[[nodiscard]] Vector3f& operator[](const int& index);

	[[nodiscard]] const Vector3f& operator[](const int& index) const;

	const Vector3f dim;
	const Vector3f* pos { nullptr };

	Vector3f min {0};
	Vector3f max {0};

	bool is_static { false };
};