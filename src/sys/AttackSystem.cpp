#include <sys/AttackSystem.hpp>
#include <iostream>
#include <util/SoundPaths.hpp>
#include <Engine/util/Math.hpp>

void AttackSystem::update(const Context& context, const float deltaTime) {
	float enemyDamage = 0.f;	// daño total que recibira el jugador despues de procesar todos los ataques
	bool  playerShoots = false;

	for (auto& data : context->getComponents().get<CharacterData>()) {
        if (data && data.attacking) {
			switch (data.getEntityType()) {
				case ENEMY :
					enemyDamage += data.attackDamage; // vamos acumulando el daño que recibe el player
					break;
				case PLAYER :
					playerShoots = true;
					break;
				default :
					std::cout << "\n\nComo vainas ataca algo que no es ni enemy ni player weon!\n\n";
					exit(-1);
			}

			data.attacking = false;
		}
    }

	if (enemyDamage != 0) {
		auto& playerHealth = context->getPlayer().getComponent<CharacterData>()->health;

		playerHealth -= enemyDamage; // para no cortar la cache desreferenciando context cada vez que nos alcance un enemy
		soundMessages.emplace_back(DAMAGE_PLAYER); // Creo el SoundMessage de Player herido

		if(!greater_e(playerHealth, 0))
		    deathMessages.emplace_back(context->getPlayer().getID());
	}



	if (playerShoots)
		context->createBullet();
}
