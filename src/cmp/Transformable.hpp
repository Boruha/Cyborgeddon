#pragma once

#include <SunlightEngine/Vector3.hpp>
#include <cmp/Component.hpp>

using Sun::Vector3f;

struct Transformable : public Component
{
	explicit Transformable(const EntityType e_type, const std::size_t e_ID, const Vector3f& pos, const Vector3f& rot)
		: Component(e_type, e_ID), position(pos), rotation(rot) {  }

	friend std::ostream& operator<<(std::ostream& os, const Transformable& trans);

	Vector3f	position {0,0,0};
	Vector3f	rotation {0,0,0};
};