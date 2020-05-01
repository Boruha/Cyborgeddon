#pragma once

#include <cmp/Component.hpp>

struct CircleBounding : Component {
	explicit CircleBounding(const EntityType e_type, const EntityID e_ID, const vec3& pos, const float rad, vec3& vel)
			: Component(e_type, e_ID), min(pos.x - rad, 0, pos.z - rad), max(pos.x + rad, 0, pos.z + rad), center(pos), radius(rad), velocity(&vel) {  }

	[[nodiscard]] constexpr static std::string_view getName() {
		return "CircleBounding";
	}

	friend std::ostream& operator<<(std::ostream& os, const CircleBounding& cmp);

	// AABB
	vec3  min {0};
	vec3  max {0};

	// circulo interior
	vec3  center {0};
	float radius {0};

	vec3 * velocity { nullptr };
};