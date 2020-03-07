#pragma once

#include <cmp/Component.hpp>
#include <util/ComponentConstants.hpp>
#include <vector>

struct AI : public Component {
	explicit AI(EntityType e_type, EntityID e_ID, const std::vector<vec3>& patrol);

	[[nodiscard]] std::string_view getName() const override {
		return "AI";
	}

	friend std::ostream& operator<<(std::ostream& os, const AI& ai);

	AI_State state { PATROL_STATE };

	//patrol var
	vec3 patrol_position[MAX_PATROL_POSITIONS] { }; // intentar hacer const

  	unsigned patrol_index { 0 };
  	unsigned max_index 	  { 0 };
	//pathing var
	int   path_index 	      { -1 };	   //vector<int> index (map).
  	int   path_node           { -1 };	   //graph index.	   

	vec3 target_position      { 0 };
};