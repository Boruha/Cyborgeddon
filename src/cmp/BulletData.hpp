#pragma once

#include <cmp/Component.hpp>

struct BulletData : Component {
	explicit BulletData(EntityType e_type, std::size_t e_ID, const int lifetime, const bool type)
		: Component(e_type, e_ID), lifetime(lifetime), type(type) {  }

	int lifetime { 15 };
	const bool type { false };
};