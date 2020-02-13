#include <sys/AttackSystem.hpp>
#include <util/SoundPaths.hpp>
#include <iostream>

void AttackSystem::update(const std::unique_ptr<GameContext>& context, const float deltaTime) {
	float enemyDamage = 0.f;	// daño total que recibira el jugador despues de procesar todos los ataques
	bool  playerShoots = false;

	for (auto& data : std::get<vector<CharacterData>>(context->getComponents(CHARACTER_DATA_TYPE))) {
        if (data && data.attacking) {
			switch (data.getEntityType()) {
				case ENEMY :
					enemyDamage += data.attackDamage; // vamos acumulando el daño que recibe el player
					soundMessages.emplace_back(ASSEMBLED_ATTACK_EVENT);
					break;
				case PLAYER :
					playerShoots = true;
					soundMessages.emplace_back(data.mode == ANGEL ? ANGEL_SHOOT_EVENT : DEMON_SHOOT_EVENT);
					break;
				default :
					std::cout << "\n\nComo vainas ataca algo que no es ni enemy ni player weon!\n\n";
					exit(-1);
			}

			data.attacking = false;
		}
    }

	context->getPlayer().characterData->health -= enemyDamage; // para no cortar la cache desreferenciando context

	if (playerShoots)
		context->createBullet();
}
