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
	os 	<< "\nAI_State: " << ai.state
		<< "\nPatrol positions: ";
	for (int i = 0; i < ai.max_index; ++i)
		os << "\n\t" << ai.patrol_position[i];
	os 	<< "\nPatrol Index: " << ai.patrol_index
		<< "\nMax Index: " << ai.max_index
		<< "\nTarget: " << ai.target_position;

	return os;
}
