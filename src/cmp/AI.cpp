#include <cmp/AI.hpp>

AI::AI(const EntityType e_type, const std::size_t e_ID, const std::vector<Vector3f>& patrol) : Component(e_type, e_ID) {
	if (patrol.size() < MAX_PATROL_POSITIONS) {
		for (const auto& pos : patrol) {
			patrol_position[max_index] = pos;
			++max_index;
		}
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
