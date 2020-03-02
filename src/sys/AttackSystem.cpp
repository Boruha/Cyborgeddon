#include <sys/AttackSystem.hpp>
#include <util/SoundPaths.hpp>
#include <Engine/util/Math.hpp>

void AttackSystem::update(const Context& context, const float deltaTime) {
	float enemyDamage = 0.f;	// daño total que recibira el jugador despues de procesar todos los ataques

	while (!damageMessages.empty()) {
		enemyDamage += damageMessages.back().damage;
		damageMessages.pop_back();
	}

	auto & player = context->getPlayer();

	if (enemyDamage != 0) {
		auto & playerHealth = player.getComponent<CharacterData>()->health;

		playerHealth -= enemyDamage;

		soundMessages.emplace_back(DAMAGE_PLAYER_EVENT); // Creo el SoundMessage de Player herido

		if(!greater_e(playerHealth, 0))
			deathMessages.emplace_back(context->getPlayer().getID());
	}

	if (player.getComponent<CharacterData>()->attacking) {
		player.getComponent<CharacterData>()->attacking = false;
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
