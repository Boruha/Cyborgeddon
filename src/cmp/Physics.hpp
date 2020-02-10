#pragma once

#include <cmp/Component.hpp>

struct Physics : public Component {
	explicit Physics(const EntityType e_type, const std::size_t e_ID, const vec3& pos, const vec3& vel, const vec3& rot)
		: Component(e_type, e_ID), position(pos), rotation(rot), velocity(vel) {  }

	friend std::ostream& operator<<(std::ostream& os, const Physics& phy);

	vec3 position;
	vec3 rotation;
	vec3 velocity {0};
};