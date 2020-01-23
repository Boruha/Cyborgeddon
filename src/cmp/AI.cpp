#include <cmp/AI.hpp>

AI::AI(const EntityType e_type, const std::size_t e_ID, const std::vector<Vector3f>& patrol) : Component(e_type, e_ID), max_index(patrol.size()) {
	if (max_index <= MAX_PATROL_POSITIONS) {
		for (int i = 0; i < max_index; ++i)
			patrol_position[i] = patrol[i];
	} else {
		std::cerr << "Se ha pasado un patrol de mas de " << MAX_PATROL_POSITIONS << " posiciones\n";
		exit(-1);
	}
}

std::ostream& operator<<(std::ostream& os, const AI &ai) {
	ai.print(os, "AI")
	<< "\n\tAI_State: " 		<< ai.state
	<< "\n\tPatrol Index: " 	<< ai.patrol_index
	<< "\n\tMax Index: " 		<< ai.max_index
	<< "\n\tTarget: " 			<< ai.target_position
	<< "\n\tPatrol positions: ";

	for (int i = 0; i < ai.max_index; ++i)
		os << "\n\t\t" << ai.patrol_position[i];

	return os;
}
