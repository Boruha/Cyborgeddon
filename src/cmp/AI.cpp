#include <cmp/AI.hpp>
#include <glm/gtx/string_cast.hpp>
#include <iostream>

AI::AI(const EntityType e_type, const EntityID e_ID, const std::vector<vec3>& patrol) : Component(e_type, e_ID), max_index(patrol.size()) {
	if (max_index <= MAX_PATROL_POSITIONS) {
		for (unsigned int i = 0; i < max_index; ++i)
			patrol_position[i] = patrol[i];
	} else {
		std::cerr << "Se ha pasado un patrol de mas de " << MAX_PATROL_POSITIONS << " posiciones\n";
		exit(-1);
	}
}

std::ostream& operator<<(std::ostream& os, const AI &ai) {
	ai.print(os, ai.getName())
	<< "\n\tPatrol Index: " 	<< ai.patrol_index
	<< "\n\tMax Index: " 		<< ai.max_index
	<< "\n\tTarget: " 			<< glm::to_string(ai.target_position)
	<< "\n\tPatrol positions: ";

	for (unsigned int i = 0; i < ai.max_index; ++i)
		os << "\n\t\t" << glm::to_string(ai.patrol_position[i]);

	return os;
}
