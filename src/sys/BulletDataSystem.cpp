#include <sys/BulletDataSystem.hpp>
#include <Engine/util/Math.hpp>

void BulletDataSystem::fixedUpdate(const Context &context, float deltaTime) {
    for (auto& bullet : context->getComponents().getComponents<BulletData>())
        if (bullet && !greater_e((bullet.distance_left -= bullet.speed * deltaTime), 0))
            deathMessages.emplace_back(bullet.getEntityID());
}

