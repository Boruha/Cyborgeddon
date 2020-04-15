#pragma once

#include <cmp/Component.hpp>

struct Transformable : public Component
{
	explicit Transformable(const EntityType e_type, const EntityID e_ID, const vec3& pos, const vec3& rot, const vec3& sca = vec3(1))
		: Component(e_type, e_ID), position(pos), rotation(rot), scale(sca) {  }

	[[nodiscard]] constexpr static std::string_view getName() {
		return "Transformable";
	}

	friend std::ostream& operator<<(std::ostream& os, const Transformable& trans);

	vec3	position { };
	vec3	rotation { };
	vec3	scale	 { };
};