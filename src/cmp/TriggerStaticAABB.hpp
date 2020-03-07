#pragma once

#include <cmp/Component.hpp>

struct TriggerStaticAABB : Component {
	explicit TriggerStaticAABB(const EntityType e_type, const EntityID e_ID, const vec3& pos, const vec3& dim, const bool pass)
			: Component(e_type, e_ID), passable(pass), min(pos - (dim * 0.5f)), max(pos + (dim * 0.5f)) {  }

	[[nodiscard]] std::string_view getName() const override {
		return "TriggerStaticAABB";
	}

	friend std::ostream& operator<<(std::ostream& os, const TriggerStaticAABB& cmp);

	bool	passable { false };
	vec3	min { 0 };
	vec3	max { 0 };
};