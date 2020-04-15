#pragma once

#include <cmp/Component.hpp>

struct Physics : public Component {
	explicit Physics(const EntityType e_type, const EntityID e_ID, const vec3& pos, const vec3& vel, const vec3& rot, const vec3& scale = vec3(1))
		: Component(e_type, e_ID), position(pos), rotation(rot), scale(scale), velocity(vel) {  }

	[[nodiscard]] constexpr static std::string_view getName() {
		return "Physics";
	}

	friend std::ostream& operator<<(std::ostream& os, const Physics& phy);

	vec3 position;
	vec3 rotation;
	vec3 scale;
	vec3 velocity {0};
};