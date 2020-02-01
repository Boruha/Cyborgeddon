#include <sys/DeathSystem.hpp>

void DeathSystem::update(const std::unique_ptr<GameContext> &context, const float deltaTime) const {
    // Mueren personajes ?
    for (const auto& data : std::get<vector<CharacterData>>(context->getComponents(CHARACTER_DATA_TYPE))) {
        if (data && !greater_e(data.health, 0)) {
            context->addToDestroy(data.getEntityID());
            if (data.getEntityType() == PLAYER) {
                std::cout << "\n\nFin de partida\n\n";    // TODO : Gestionar fin de partida
                exit(0);
            }
        }
    }

    // Mueren balas ?
    for (auto& bullet : std::get<vector<BulletData>>(context->getComponents(BULLET_DATA_TYPE))) {
        if (bullet) {
            if (!greater_e((bullet.distance_left -= bullet.speed * deltaTime), 0)) {
                context->addToDestroy(bullet.getEntityID());
            }
        }
    }
}
