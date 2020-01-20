#pragma once

#include <cmp/Component.hpp>

struct BulletData : Component {
	explicit BulletData(EntityType e_type, std::size_t e_ID, const float speed, const bool damageType, const float damage)
		: Component(e_type, e_ID), damageType(damageType), speed(speed), damage(damage) {  }

	friend std::ostream& operator<<(std::ostream& os, const BulletData& data);

	const bool  damageType { false };
	const float speed { 100.f };
	const float damage { 0.f };
	float 		distance_left { 100.f };
};