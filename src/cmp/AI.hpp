#pragma once

#include <cmp/Component.hpp>
#include <util/ComponentConstants.hpp>
#include <vector>

struct AI : public Component {
	explicit AI(EntityType e_type, EntityID e_ID, const std::vector<vec3>& patrol, unsigned phase);

	[[nodiscard]] std::string_view getName() const override {
		return "AI";
	}

	friend std::ostream& operator<<(std::ostream& os, const AI& ai);

	//PATROL
	vec3 patrol_position[MAX_PATROL_POSITIONS] { }; // intentar hacer const

  	unsigned patrol_index     { 0 };
  	unsigned max_index 	      { 0 };
	
	//PATH
	int      path_index 	  { -1 };	   //vector<int> index (map).
  	int      path_node        { -1 };	   //graph index.	   

	vec3     target_position  { 0 };

	//AI_STATE for Scheduling
	unsigned frequecy_state   { PATROL_STATE };
	unsigned scheduling_phase { 0 };
	bool     scheduled        { false };
};