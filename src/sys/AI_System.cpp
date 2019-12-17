#include <sys/AI_System.hpp>

// TODO: considerar los estados de la IA como punteros a funcion
void AI_System::update(const std::unique_ptr<GameContext>& context) const {
	for (auto &enemy : context->getEnemies()) {
		enemy->velocity.direction = context->getPlayer()->transformable.position - enemy->transformable.position;
		enemy->velocity.direction.y = 0; // anulo la y porque las alturas son distintas

		enemy->transformable.rotation.y = (enemy->velocity.direction).getRotationY();
		enemy->node.setRotation(enemy->transformable.rotation);

		if ((enemy->velocity.direction).length() > 25) {
			enemy->ai_state = 1;
			(enemy->velocity.direction).normalize();
		} else {
			enemy->ai_state = 0;
			enemy->velocity.direction = 0;
		}
	}
}