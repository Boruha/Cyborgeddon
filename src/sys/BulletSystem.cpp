#include <sys/BulletSystem.hpp>

void BulletSystem::update(const std::unique_ptr<GameContext> &context, const float deltaTime) const {
	for (auto& cmp : std::get<vector<BulletData>>(context->getComponents(BULLET_DATA_TYPE)))
		if (cmp.getEntityType() != UNDEFINED)
			if (!Sun::greater_e((cmp.distance_left -= cmp.speed * deltaTime), 0))
				context->addToDestroy(cmp.getEntityID());
}