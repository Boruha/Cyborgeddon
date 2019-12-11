#include <sys/AI_System.hpp>
#include <cmath>

// TODO: considerar los estados de la IA como punteros a funcion
void AI_System::update(const std::vector<std::unique_ptr<EntityPlayer>> &players,
					   const std::vector<std::unique_ptr<EntityEnemy>> &enemies) {

	for (auto &player : players) {
		for (auto &enemy : enemies) {
			enemy->velocity.direction = player->transformable.position - enemy->transformable.position;
			rotating(enemy);

			if ((enemy->velocity.direction).length() > 25) {
				enemy->ai_state = 1;
				(enemy->velocity.direction).normalize();
			} else {
				enemy->ai_state = 0;
				enemy->velocity.direction = 0;
			}
		}
	}
}

void AI_System::rotating(const std::unique_ptr<EntityEnemy>& enemy) const {
	enemy->transformable.rotation.y = (enemy->velocity.direction).getRotationY();
	enemy->node.setRotation(enemy->transformable.rotation);
}