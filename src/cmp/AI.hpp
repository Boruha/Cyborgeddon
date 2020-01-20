#pragma once

#include <cstdlib>
#include <vector>
#include <SunlightEngine/Vector3.hpp>
#include <cmp/Component.hpp>
#include <util/Alias.hpp>
#include <util/Constants.hpp>

using Sun::Vector3f;

struct AI : public Component {
	explicit AI(EntityType e_type, std::size_t e_ID, const std::vector<Vector3f>& patrol);

	friend std::ostream& operator<<(std::ostream& os, const AI& ai);

	AI_State state { PATROL_STATE };

	Vector3f patrol_position[MAX_PATROL_POSITIONS]; // intentar hacer const

		  unsigned short patrol_index { 0 };
	const unsigned short max_index 	  { 0 };

	Vector3f target_position {0};
};