#pragma once

#include <cmp/Component.hpp>

struct TriggerFastMov : Component {
	explicit TriggerFastMov(const EntityType e_type, const EntityID e_ID, const vec3& pos, vec3& vel)
			: Component(e_type, e_ID), position(pos), velocity(vel) {  }

	[[nodiscard]] std::string_view getName() const override {
		return "TriggerFastMov";
	}

	friend std::ostream& operator<<(std::ostream& os, const TriggerFastMov& cmp);

	vec3   	position { 0 };		// posicion del bounding
	vec3  	velocity { 0 }; 		// velocity del physics cmp
};