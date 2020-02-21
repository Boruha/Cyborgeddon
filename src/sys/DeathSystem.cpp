#include <sys/DeathSystem.hpp>
#include <Engine/util/Math.hpp>

void DeathSystem::update(const std::unique_ptr<GameContext> &context, const float deltaTime) {
	std::array<EntityID, 16> entitiesToDestroy { }; // suponemos que como mucho pueden morir en una iteracion 16 entidades (que creo que ya es mucho y no habra problema)
	unsigned numToDestroy = 0;

    // Mueren personajes ?
    for (const auto& data : context->getComponents().get<CharacterData>())
        if (data && !greater_e(data.health, 0))
        	entitiesToDestroy[numToDestroy++] = data.getEntityID();

    // Mueren balas ?
    for (auto& bullet : context->getComponents().get<BulletData>())
        if (bullet && !greater_e((bullet.distance_left -= bullet.speed * deltaTime), 0))
				entitiesToDestroy[numToDestroy++] = bullet.getEntityID();

    for (unsigned i = 0; i < numToDestroy; i++)
    	context->addToDestroy(entitiesToDestroy[i]);
}
