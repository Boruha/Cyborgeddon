#pragma once

#include <cstdlib>
#include <vector>
#include <SunlightEngine/Vector3.hpp>
#include <cmp/Component.hpp>
#include <util/Alias.hpp>

using Sun::Vector3f;

struct AI : public Component {
	explicit AI(EntityType e_type, std::size_t e_ID, const std::vector<Vector3f>& patrol);
	~AI();

	AI_State state { PATROL_STATE };

	std::vector<Vector3f> patrol_position;
	std::size_t patrol_index { 0 };

	Vector3f target_position {0};

	constexpr static std::size_t MAX_PATROL_POSITIONS = 8;
};