#include <sys/AttackSystem.hpp>
#include <Engine/util/Math.hpp>

void AttackSystem::fixedUpdate(const Context& context, float deltaTime) {
	float enemyDamage = 0.f;	// daño total que recibira el jugador despues de procesar todos los ataques

	while (!damageMessages.empty()) {
		enemyDamage += damageMessages.back().damage;
		damageMessages.pop_back();
	}

	auto & playerData = *context->getPlayer().getComponent<CharacterData>();

	if (enemyDamage != 0) {
		//playerData.health -= enemyDamage;

		soundMessages.emplace_back(DAMAGE_PLAYER); // Creo el SoundMessage de Player herido


		if(!greater_e(playerData.health, 0))
			deathMessages.emplace_back(context->getPlayer().getID());
	}

	if (playerData.attacking) {
		playerData.attacking = false;
		context->createBullet();
	}
}

void AttackSystem::init() {
	damageMessages.reserve(16);
}

void AttackSystem::reset() {
	damageMessages.clear();
	damageMessages.reserve(16);
}
