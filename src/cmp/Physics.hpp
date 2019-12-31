#pragma once

#include <SunlightEngine/Vector3.hpp>
#include <cmp/Transformable.hpp>
#include <cmp/Component.hpp>

using Sun::Vector3f;

struct Physics : public Component {
	explicit Physics(const EntityType e_type, const std::size_t e_ID, const Vector3f& pos, const Vector3f& vel, const Vector3f& rot)
		: Component(e_type, e_ID), position(pos), rotation(rot), velocity(vel) {  }

	Vector3f position;
	Vector3f rotation;
	Vector3f velocity {0};
};