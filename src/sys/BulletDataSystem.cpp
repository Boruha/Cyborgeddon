#include <sys/BulletDataSystem.hpp>
#include <Engine/util/Math.hpp>

void BulletDataSystem::update(const Context &context, const float deltaTime) {
    // Mueren balas ?
    for (auto& bullet : context->getComponents().get<BulletData>())
        if (bullet && !greater_e((bullet.distance_left -= bullet.speed * deltaTime), 0))
            deathMessages.emplace_back(bullet.getEntityID());
}

