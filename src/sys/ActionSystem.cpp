#include <sys/ActionSystem.hpp>

void ActionSystem::update(const std::unique_ptr<GameContext>& context, const float deltaTime) const {
    for (const auto& data : std::get<vector<CharacterData>>(context->getComponents(CHARACTER_DATA_TYPE))) {
        if (data) {
            if (data.attacking) {
                switch (data.getEntityType()) {
                    case ENEMY :
                        context->getPlayer().characterData->health -= data.attackDamage;
                        std::cout << "\n\nPLAYER RECIBE UN ATAQUE" << *(context->getPlayer().characterData);
                        break;
                    case PLAYER :
                        context->createBullet();
                        break;
                    default :
                        std::cout << "\n\nComo vainas ataca algo que no es ni enemy ni player weon!\n\n";
                        exit(-1);
                }
            }
        }
    }
}
