#pragma once

#include <cmp/Component.hpp>
#include <util/ComponentConstants.hpp>
#include <vector>

struct AI : public Component {
	explicit AI(EntityType e_type, std::size_t e_ID, const std::vector<vec3>& patrol);

	friend std::ostream& operator<<(std::ostream& os, const AI& ai);

	AI_State state { PATROL_STATE };

	vec3 patrol_position[MAX_PATROL_POSITIONS]; // intentar hacer const

  	unsigned short patrol_index { 0 };
  	unsigned short max_index 	{ 0 };

	vec3 target_position {0};
};