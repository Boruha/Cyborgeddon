#pragma once

#include <SunlightEngine/Vector3.hpp>

using Sun::Vector3f;

struct Velocity : public Component
{
	explicit Velocity(const EntityType e_type, const std::size_t e_ID, const float defaultSpeed, const float acceleration)
		: Component(e_type, e_ID), speed(defaultSpeed), defaultSpeed(defaultSpeed), acceleration(acceleration) {  }

	explicit Velocity(const EntityType e_type, const std::size_t e_ID, const Vector3f& dir, const float defaultSpeed, const float acceleration)
		: Component(e_type, e_ID), direction(dir), speed (defaultSpeed), defaultSpeed(defaultSpeed), acceleration(acceleration) {  }

	Vector3f	direction {0};
	float  		speed {0};
	const float defaultSpeed {0};
	float 		acceleration {0};
};