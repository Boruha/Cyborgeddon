#include <sys/CharacterDataSystem.hpp>

void CharacterDataSystem::update(const std::unique_ptr<GameContext>& context, const float deltaTime) const {
	for (auto& data : std::get<vector<CharacterData>>(context->getComponents(CHARACTER_DATA_TYPE))) { // TODO : gestionar cmp ataque
        if (data && data.getEntityType() == ENEMY && data.attacking) {
            context->getPlayer().characterData->health -= data.attackDamage;

            std::cout << "\n\nPLAYER RECIBE UN ATAQUE" << *(context->getPlayer().characterData);

            if (!Sun::greater_e(context->getPlayer().characterData->health, 0.f)) {
                std::cout << "\n\nFin de partida\n\n";    // TODO : Gestionar fin de partida
                exit(0);
            }
        }
    }

    for (const auto& data : std::get<vector<CharacterData>>(context->getComponents(CHARACTER_DATA_TYPE))) // TODO : gestionar cmp vida por separado de ataque
        if (data && !Sun::greater_e(data.health, 0))
            context->addToDestroy(data.getEntityID());
}
