#pragma once

#include <cmp/Component.hpp>

struct RigidStaticAABB : Component {
	explicit RigidStaticAABB(const EntityType e_type, const EntityID e_ID, const vec3& pos, const vec3& dim)
			: Component(e_type, e_ID), min(pos - (dim * 0.5f)), max(pos + (dim * 0.5f)) {  }

	friend std::ostream& operator<<(std::ostream& os, const RigidStaticAABB& cmp);

	vec3	min { 0 };
	vec3	max { 0 };
};