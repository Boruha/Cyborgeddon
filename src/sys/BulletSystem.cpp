#include <sys/BulletSystem.hpp>

void BulletSystem::update(const std::unique_ptr<GameContext> &context, const float deltaTime) const {
	for (auto& cmp : context->getBulletDataComponents())
		if (cmp.getEntityType() != UNDEFINED)
			if ((cmp.distance_left -= cmp.speed * deltaTime) <= 0)
				context->addToDestroy(cmp.getEntityID());

	for (auto& ent : context->getEntities())
		if(ent.getType() != UNDEFINED && ent.getType() == BULLET)
			ent.physics->velocity = ent.velocity->direction * ent.velocity->speed * deltaTime;
}