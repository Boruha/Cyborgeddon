#include <sys/BulletSystem.hpp>

void BulletSystem::update(const std::unique_ptr<GameContext>& context) const {
	for (auto& cmp : context->getBulletDataComponents())
		if (cmp.getEntityType() != UNDEFINED)
			if (--cmp.lifetime <= 0)
				context->addToDestroy(cmp.getEntityID());
}