#include <cmp/AI.hpp>

AI::AI(const EntityType e_type, const std::size_t e_ID, const std::vector<Vector3f>& patrol) : Component(e_type, e_ID) {
	patrol_position.reserve(MAX_PATROL_POSITIONS);

	for (const auto& pos : patrol) {
		patrol_position.emplace_back(pos);
	}
}

AI::~AI() {
	patrol_position.clear();
}