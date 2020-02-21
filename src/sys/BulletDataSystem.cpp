#include <sys/BulletDataSystem.hpp>
#include <Engine/util/Math.hpp>

void BulletDataSystem::update(const std::unique_ptr<GameContext> &context, const float deltaTime) {
    // Mueren balas ?
    for (auto& bullet : std::get<vector<BulletData>>(context->getComponents(BULLET_DATA_TYPE)))
        if (bullet && !greater_e((bullet.distance_left -= bullet.speed * deltaTime), 0))
            deathMessages.emplace_back(bullet.getEntityID());
}

