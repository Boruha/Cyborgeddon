#include <sys/ActionSystem.hpp>
#include <iostream>

void ActionSystem::update(const std::unique_ptr<GameContext>& context, const float deltaTime) const {
	float enemyDamage = 0.f;	// daño total que recibira el jugador despues de procesar todos los ataques
	bool  playerShoots = false;

	for (const auto& data : std::get<vector<CharacterData>>(context->getComponents(CHARACTER_DATA_TYPE))) {
        if (data && data.attacking) {
			switch (data.getEntityType()) {
				case ENEMY :
					enemyDamage += data.attackDamage;
					break;
				case PLAYER :
					playerShoots = true;
					break;
				default :
					std::cout << "\n\nComo vainas ataca algo que no es ni enemy ni player weon!\n\n";
					exit(-1);
			}
		}
    }

	context->getPlayer().characterData->health -= enemyDamage;

	if (playerShoots)
		context->createBullet();
}
