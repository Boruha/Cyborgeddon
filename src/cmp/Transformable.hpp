#pragma once

#include <cmp/Component.hpp>

struct Transformable : public Component
{
	explicit Transformable(const EntityType e_type, const std::size_t e_ID, const vec3& pos, const vec3& rot)
		: Component(e_type, e_ID), position(pos), rotation(rot) {  }

	friend std::ostream& operator<<(std::ostream& os, const Transformable& trans);

	vec3	position {0,0,0};
	vec3	rotation {0,0,0};
};