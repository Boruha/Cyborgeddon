#pragma once

#include <cstdlib>
#include <vector>
#include <SunlightEngine/Vector3.hpp>

using Sun::Vector3f;

enum AI_State : std::size_t { PATROL_STATE, PURSUE_STATE, ATTACK_STATE, END_STATE }; // END_STATE debe estar S I E M P R E al final

struct AI {
	explicit AI(const std::vector<Vector3f>& patrol);
	~AI();

	AI_State state { PATROL_STATE };

	std::vector<Vector3f> patrol_position;
	std::size_t patrol_index { 0 };

	Vector3f target_position {0};

	constexpr static size_t MAX_PATROL_POSITIONS = 8;
};