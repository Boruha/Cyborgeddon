#pragma once

#include <cmp/Component.hpp>

struct BulletData : Component {
	explicit BulletData(EntityType e_type, std::size_t e_ID, const float speed, const bool damageType)
		: Component(e_type, e_ID), speed(speed), damageType(damageType) {  }

	const float speed { 100.f };
	const bool  damageType { false };
	float distance_left { 100.f };
};