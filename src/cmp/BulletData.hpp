#pragma once

#include <cmp/Component.hpp>

struct BulletData : Component {
	explicit BulletData(EntityType e_type, EntityID e_ID, const float speed, const ModeType damageType, const float damage)
		: Component(e_type, e_ID), damageType(damageType), speed(speed), damage(damage) {  }

	[[nodiscard]] constexpr static std::string_view getName() {
		return "BulletData";
	}

	friend std::ostream& operator<<(std::ostream& os, const BulletData& data);

	ModeType  	damageType;
	float 		speed 			{ 100.f };
	float 		damage 			{  0.f  };
	float 		distance_left 	{ 100.f };
};