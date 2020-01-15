#pragma once

#include <SunlightEngine/Vector3.hpp>
#include <cmp/Component.hpp>

using Sun::Vector3f;

struct Velocity : public Component
{
	explicit Velocity(const EntityType e_type, const std::size_t e_ID, const float defaultSpeed, const float acceleration)
		: Component(e_type, e_ID), currentSpeed(defaultSpeed), defaultSpeed(defaultSpeed), acceleration(acceleration) {  }

	explicit Velocity(const EntityType e_type, const std::size_t e_ID, const Vector3f& dir, const float defaultSpeed, const float acceleration)
		: Component(e_type, e_ID), direction(dir), currentSpeed (defaultSpeed), defaultSpeed(defaultSpeed), acceleration(acceleration) {  }

	friend std::ostream& operator<<(std::ostream& os, const Velocity& vel);

	Vector3f	direction 	 {0};
	float  		currentSpeed {0};
	float 		defaultSpeed {0};
	float 		acceleration {0};
};