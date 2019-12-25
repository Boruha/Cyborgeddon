#include <cmp/AI.hpp>

AI::AI(const std::vector<Vector3f>& patrol) {
	patrol_position.reserve(MAX_PATROL_POSITIONS);

	for (const auto& pos : patrol) {
		patrol_position.emplace_back(pos);
	}
}

AI::~AI() {
	patrol_position.clear();
}