#include <sys/DeathSystem.hpp>
#include <Engine/util/Math.hpp>
#include <iostream>

void DeathSystem::update(const std::unique_ptr<GameContext> &context, const float deltaTime) const {
	std::array<EntityID, 16> entitiesToDestroy { }; // suponemos que como mucho pueden morir en una iteracion 16 entidades (que creo que ya es mucho y no habra problema)
	unsigned numToDestroy = 0;
    // Mueren personajes ?
    for (const auto& data : std::get<vector<CharacterData>>(context->getComponents(CHARACTER_DATA_TYPE))) {
        if (data && !greater_e(data.health, 0)) {
        	entitiesToDestroy[numToDestroy++] = data.getEntityID();
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
				entitiesToDestroy[numToDestroy++] = bullet.getEntityID();
            }
        }
    }

    for (unsigned i = 0; i < numToDestroy; i++) {
    	std::cout << "Se elimina entidad con id " << entitiesToDestroy[i] << std::endl;
    	context->addToDestroy(entitiesToDestroy[i]);
    }
}
