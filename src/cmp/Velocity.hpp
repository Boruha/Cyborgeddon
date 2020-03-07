#pragma once

#include <cmp/Component.hpp>

struct Velocity : public Component
{
	explicit Velocity(const EntityType e_type, const EntityID e_ID, const float defaultSpeed, const float acceleration)
		: Component(e_type, e_ID), currentSpeed(defaultSpeed), defaultSpeed(defaultSpeed), acceleration(acceleration) {  }

	explicit Velocity(const EntityType e_type, const EntityID e_ID, const vec3& dir, const float defaultSpeed, const float acceleration)
		: Component(e_type, e_ID), direction(dir), currentSpeed (defaultSpeed), defaultSpeed(defaultSpeed), acceleration(acceleration) {  }

	[[nodiscard]] std::string_view getName() const override {
		return "Velocity";
	}

	friend std::ostream& operator<<(std::ostream& os, const Velocity& vel);

	vec3		direction {0};
	float 	 		currentSpeed {0};
	float		 	defaultSpeed {0};
	float 			acceleration {0};
};